package hardware

import (
	"cnc/client/communication"
	"encoding/json"
	"fmt"
)

func (i *Movements) Init(connection communication.Connection) {
	i.connection = connection
}

func (i *Movements) Move(movements []MovementConfiguration, repeats uint) error {
	config := map[string]interface{}{
		"repeats":   repeats,
		"movements": movements,
	}
	body := map[string]interface{}{
		"id":     "",
		"config": config,
	}
	request := communication.Request{
		Route:  "movements",
		Method: "POST",
		Body:   body,
	}
	resp, err := i.connection.RunRequest(request)
	if err != nil {
		return err
	}
	if resp.ResultCode != 0 {
		response_bytes, err := json.Marshal(resp.Body)
		if err != nil {
			return err
		}
		response_unmarshalled := make(map[string]interface{}, 0)
		err = json.Unmarshal(response_bytes, &response_unmarshalled)
		if err != nil {
			return err
		}
		return fmt.Errorf("a failure occured while performing request: %v", response_unmarshalled["what"])
	}
	return nil
}
