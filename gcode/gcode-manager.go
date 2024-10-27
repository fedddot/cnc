package gcode

import (
	"cnc/client/manager"
	"cnc/client/model"
	"fmt"
	"strconv"
	"strings"
)

type GcodeManager struct {
	default_feed float32
	fast_feed    float32

	location_mode LocationMode
	position      model.Vector[float32]

	linear_movement *manager.LinearMovement
}

func (i *GcodeManager) Init(linear_movement *manager.LinearMovement) error {
	if linear_movement == nil {
		return fmt.Errorf("invalid linear movement ptr received")
	}
	i.default_feed = float32(10)
	i.fast_feed = float32(20)
	i.location_mode = ABSOLUTE
	i.position = model.Vector[float32]{X: 0.0, Y: 0.0, Z: 0.0}
	i.linear_movement = linear_movement
	return nil
}

func parseTokens(command string) []string {
	tokens := make([]string, 0)
	for _, substr := range strings.Split(command, " ") {
		if substr == "" {
			continue
		}
		tokens = append(tokens, substr)
	}
	return tokens
}

func (i GcodeManager) parseMovementDescriptor(command_id CommandId, tokens []string) (MovementDescriptor, error) {
	res := MovementDescriptor{}
	coordinates := make(map[model.Dimension]float32, 0)
	feed := i.default_feed
	for _, token := range tokens {
		if len(token) < 2 {
			return res, fmt.Errorf("token %s is too short", token)
		}
		value, err := strconv.ParseFloat(token[1:], 32)
		if err != nil {
			return res, fmt.Errorf("failed to parse a number from %s", token[1:])
		}
		switch token[0] {
		case 'X':
			coordinates[model.X] = float32(value)
		case 'Y':
			coordinates[model.Y] = float32(value)
		case 'Z':
			coordinates[model.Z] = float32(value)
		case 'F':

			feed = float32(value)
		default:
			continue
		}
	}
	switch command_id {
	case G00:
		feed = i.fast_feed
	case G28:
		feed = i.fast_feed
	}
	res.Coordinates = coordinates
	res.Feed = feed
	return res, nil
}

func (i GcodeManager) parseCircularMovementDescriptor(command_id CommandId, tokens []string) (CircularMovementDescriptor, error) {
	res := CircularMovementDescriptor{}
	movement_descriptor, err := i.parseMovementDescriptor(command_id, tokens)
	if err != nil {
		return res, err
	}
	res.MovementDescriptor = movement_descriptor
	switch command_id {
	case G02:
		res.Direction = CW
	case G03:
		res.Direction = CCW
	default:
		return res, fmt.Errorf("non-circular command: %s", command_id)
	}
	center_coordinates := make(map[model.Dimension]float32, 0)
	for _, token := range tokens {
		if len(token) < 2 {
			return res, fmt.Errorf("token %s is too short", token)
		}
		value, err := strconv.ParseFloat(token[1:], 32)
		if err != nil {
			return res, fmt.Errorf("failed to parse a number from %s", token[1:])
		}
		switch token[0] {
		case 'I':
			center_coordinates[model.X] = float32(value)
		case 'J':
			center_coordinates[model.Y] = float32(value)
		case 'K':
			center_coordinates[model.Z] = float32(value)
		default:
			continue
		}
	}
	res.RotationCenter = center_coordinates
	return res, nil
}

func (i GcodeManager) generateLinearMovementVector(descriptor MovementDescriptor) (model.Vector[float32], error) {
	result := model.Vector[float32]{}
	switch i.location_mode {
	case ABSOLUTE:
		result = i.position
		for dim, coord := range descriptor.Coordinates {
			result.Set(dim, coord)
		}
		result = result.Add(i.position.Negate())
	case RELATIVE:
		result = model.Vector[float32]{X: 0, Y: 0, Z: 0}
		for dim, coord := range descriptor.Coordinates {
			result.Set(dim, coord)
		}
	default:
		return result, fmt.Errorf("unsupported location mode: %d", i.location_mode)
	}
	return result, nil
}

func (i *GcodeManager) runLinearMovementCommand(command_id CommandId, descriptor MovementDescriptor) error {
	movement_vector, err := i.generateLinearMovementVector(descriptor)
	if err != nil {
		return err
	}
	err = i.linear_movement.Move(movement_vector, descriptor.Feed)
	if err != nil {
		return err
	}
	i.position = i.position.Add(movement_vector)
	return nil
}

func runCircularMovementCommand(command_id CommandId, descriptor CircularMovementDescriptor) error {
	return fmt.Errorf("NOT IMPLEMENTED")
}

func (i *GcodeManager) RunCommand(command string) error {
	tokens := parseTokens(command)
	if len(tokens) < 1 {
		return fmt.Errorf("empty command received")
	}
	command_id_token := CommandId(tokens[0])
	switch command_id_token {
	case G00:
		descriptor, err := i.parseMovementDescriptor(command_id_token, tokens[1:])
		if err != nil {
			return err
		}
		return i.runLinearMovementCommand(command_id_token, descriptor)
	case G01:
		descriptor, err := i.parseMovementDescriptor(command_id_token, tokens[1:])
		if err != nil {
			return err
		}
		return i.runLinearMovementCommand(command_id_token, descriptor)
	case G02:
		descriptor, err := i.parseCircularMovementDescriptor(command_id_token, tokens[1:])
		if err != nil {
			return err
		}
		return runCircularMovementCommand(command_id_token, descriptor)
	case G03:
		descriptor, err := i.parseCircularMovementDescriptor(command_id_token, tokens[1:])
		if err != nil {
			return err
		}
		return runCircularMovementCommand(command_id_token, descriptor)
	case G90:
		i.location_mode = ABSOLUTE
		return nil
	case G91:
		i.location_mode = RELATIVE
		return nil
	default:
		return fmt.Errorf("unsupported command id: %s", command_id_token)
	}
}
