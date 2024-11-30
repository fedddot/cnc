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
	Direction GpioDirection `json:"direction"`
}

type GpioCreateConfig struct {
	Id     string     `json:"id"`
	Config GpioConfig `json:"config"`
}

type Gpio struct {
	connection communication.Connection
	create_cfg GpioCreateConfig
}

func (i *Gpio) Init(create_cfg GpioCreateConfig, connection communication.Connection) error {
	request := communication.Request{
		Route:  "gpios",
		Method: "POST",
		Body:   create_cfg,
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
	i.create_cfg = create_cfg
	return nil
}

func (i Gpio) Uninit() error {
	body := make(map[string]interface{}, 0)
	request := communication.Request{
		Route:  fmt.Sprintf("gpios/%s", i.create_cfg.Id),
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
