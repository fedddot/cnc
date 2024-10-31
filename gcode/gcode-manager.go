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

	linear_movement   manager.IMovement
	circular_movement manager.IMovement
}

func (i *GcodeManager) Init(linear_movement manager.IMovement, circular_movement manager.IMovement) error {
	i.default_feed = float32(10)
	i.fast_feed = float32(20)
	i.location_mode = ABSOLUTE
	i.position = model.Vector[float32]{X: 0.0, Y: 0.0, Z: 0.0}
	i.linear_movement = linear_movement
	i.circular_movement = circular_movement
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

func (i GcodeManager) getTargetVectorFromReceivedCoordinates(coordinates map[model.Dimension]float32) model.Vector[float32] {
	res := model.Vector[float32]{X: 0, Y: 0, Z: 0}
	for dim, value := range coordinates {
		if i.location_mode == ABSOLUTE {
			current_pos, _ := i.position.Get(dim)
			res.Set(dim, value-current_pos)
		} else {
			res.Set(dim, value)
		}
	}
	return res
}

func (i GcodeManager) parseMovementDescriptor(command_id CommandId, tokens []string) (MovementDescriptor, error) {
	res := MovementDescriptor{}
	target_coordinates := make(map[model.Dimension]float32, 0)
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
			target_coordinates[model.X] = float32(value)
		case 'Y':
			target_coordinates[model.Y] = float32(value)
		case 'Z':
			target_coordinates[model.Z] = float32(value)
		case 'F':
			feed = float32(value)
		default:
			continue
		}
	}
	target := i.getTargetVectorFromReceivedCoordinates(target_coordinates)
	switch command_id {
	case G00:
		feed = i.fast_feed
	case G28:
		feed = i.fast_feed
	}
	res.Target = target
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
		res.Direction = manager.CW
	case G03:
		res.Direction = manager.CCW
	default:
		return res, fmt.Errorf("non-circular command: %s", command_id)
	}
	rotation_center := model.Vector[float32]{X: 0, Y: 0, Z: 0}
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
			rotation_center.X = float32(value)
		case 'J':
			rotation_center.Y = float32(value)
		case 'K':
			rotation_center.Z = float32(value)
		default:
			continue
		}
	}
	res.RotationCenter = rotation_center
	return res, nil
}

func (i *GcodeManager) runLinearMovementCommand(command_id CommandId, descriptor MovementDescriptor) error {
	linear_movement := i.linear_movement.(*manager.LinearMovement)
	err := (*i.linear_movement).(manager.LinearMovement).Move(descriptor.Target, descriptor.Feed)
	if err != nil {
		return err
	}
	i.position = i.position.Add(descriptor.Target)
	return nil
}

func (i *GcodeManager) runCircularMovementCommand(command_id CommandId, descriptor CircularMovementDescriptor) error {
	err := i.circular_movement.Move(descriptor.Target, descriptor.RotationCenter, descriptor.Direction, descriptor.Feed)
	if err != nil {
		return err
	}
	i.position = i.position.Add(descriptor.Target)
	return nil
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
		return i.runCircularMovementCommand(command_id_token, descriptor)
	case G03:
		descriptor, err := i.parseCircularMovementDescriptor(command_id_token, tokens[1:])
		if err != nil {
			return err
		}
		return i.runCircularMovementCommand(command_id_token, descriptor)
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
