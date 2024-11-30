package hardware

import (
	"cnc/client/communication"
	"encoding/json"
	"fmt"
)

type GpioNumber int

type GpioDirection int

const (
	IN  GpioDirection = 0
	OUT GpioDirection = 1
)

type GpioState int

const (
	LOW  GpioState = 0
	HIGH GpioState = 1
)

type Gpio struct {
	connection communication.Connection
	id         string
}

func (i *Gpio) Init(id string, num GpioNumber, dir GpioDirection, connection communication.Connection) error {
	gpio_cfg := make(map[string]interface{}, 0)
	gpio_cfg["gpio_id"] = num
	gpio_cfg["dir"] = dir

	request_body := make(map[string]interface{}, 0)
	request_body["id"] = id
	request_body["config"] = gpio_cfg

	request := communication.Request{
		Route:  "gpios",
		Method: "POST",
		Body:   request_body,
	}
	resp, err := connection.RunRequest(request)
	if err != nil {
		return err
	}
	if resp.ResultCode != 200 {
		response_body, _ := json.Marshal(resp.Body)
		return fmt.Errorf("server returned failure code: %d; %s", resp.ResultCode, response_body)
	}

	i.connection = connection
	i.id = id
	return nil
}

func (i Gpio) Uninit() error {
	request_body := make(map[string]interface{}, 0)
	request := communication.Request{
		Route:  fmt.Sprintf("gpios/%s", i.id),
		Method: "DELETE",
		Body:   request_body,
	}
	resp, err := i.connection.RunRequest(request)
	if err != nil {
		return err
	}
	if resp.ResultCode != 200 {
		response_body, _ := json.Marshal(resp.Body)
		return fmt.Errorf("server returned failure code: %d; %s", resp.ResultCode, response_body)
	}
	return nil
}
