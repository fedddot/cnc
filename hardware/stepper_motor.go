package hardware

import (
	"cnc/client/communication"
	"fmt"
)

type StepperMotor struct {
	id         string
	connection communication.Connection
}

func (i *StepperMotor) Init(gpo_map map[string]int, id string, connection communication.Connection) error {
	if connection == nil {
		return fmt.Errorf("invalid connection received")
	}
	i.id = id
	i.connection = connection
	body := make(map[string]interface{}, 0)
	for gpo_tag, gpo_id := range gpo_map {
		body[gpo_tag] = gpo_id
	}
	body["id"] = id
	response, err := i.connection.RunRequest(
		communication.Request{
			Route:  "steppers",
			Method: "POST",
			Body:   body,
		},
	)
	if err != nil {
		return err
	}
	if response.ResultCode != 200 {
		return fmt.Errorf("failed to create motor %s, result code: %d", id, response.ResultCode)
	}
	return nil
}

func (i *StepperMotor) Uninit() error {
	body := make(map[string]interface{}, 0)
	response, err := i.connection.RunRequest(
		communication.Request{
			Route:  fmt.Sprintf("steppers/%s", i.id),
			Method: "DELETE",
			Body:   body,
		},
	)
	if err != nil {
		return err
	}
	if response.ResultCode != 200 {
		return fmt.Errorf("failed to delete motor %s, result code: %d", i.id, response.ResultCode)
	}
	return nil
}

func (i *StepperMotor) Steps(steps_num int, direction string, time_on_us int, time_off_us int) error {
	body := make(map[string]interface{}, 0)
	body["steps_num"] = steps_num
	switch direction {
	case "CW":
		body["dir"] = 0
	case "CCW":
		body["dir"] = 1
	default:
		return fmt.Errorf("unsupported direction received: %s", direction)
	}
	body["time_on_ms"] = time_on_us
	body["time_off_ms"] = time_off_us
	response, err := i.connection.RunRequest(
		communication.Request{
			Route:  fmt.Sprintf("steppers/%s", i.id),
			Method: "PUT",
			Body:   body,
		},
	)
	if err != nil {
		return err
	}
	if response.ResultCode != 200 {
		return fmt.Errorf("failed to update motor %s, result code: %d", i.id, response.ResultCode)
	}
	return nil
}
