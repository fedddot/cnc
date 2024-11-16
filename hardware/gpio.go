package hardware

import (
	"cnc/client/communication"
	"encoding/json"
	"fmt"
)

type GpioDirection int

const (
	IN  GpioDirection = 0
	OUT GpioDirection = 1
)

type GpioConfig struct {
	GpioId    int           `json:"gpio_id"`
	Direction GpioDirection `json:"dir"`
}

type Gpio struct {
	connection communication.Connection
	id         string
}

func (i *Gpio) Init(id string, cfg GpioConfig, connection communication.Connection) error {
	body := make(map[string]interface{}, 0)
	body["id"] = id
	body["config"] = cfg
	request := communication.Request{
		Route:  "gpios",
		Method: "POST",
		Body:   body,
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
	body := make(map[string]interface{}, 0)
	request := communication.Request{
		Route:  fmt.Sprintf("gpios/%s", i.id),
		Method: "DELETE",
		Body:   body,
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
