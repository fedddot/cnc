package hardware

import (
	"cnc/client/communication"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestStepperMotor_Init_Uninit(t *testing.T) {
	// GIVEN
	connection := communication.HttpConnection{}
	test_cfg := StepperMotorConfig{
		ControlOutputs: StepperMotorControlOutputs{
			ENA: 10,
			ENB: 11,
		},
		DirectionOutputs: StepperMotorDirectionOutputs{
			ATop: 12,
			ABtm: 13,
			BTop: 14,
			BBtm: 15,
		},
	}
	test_id := StepperMotorId("test_stepper_motor")

	// WHEN
	connection.Init("http://127.0.0.1", "5000")
	instance := StepperMotor{}

	// THEN
	err := instance.Init(test_id, test_cfg, &connection)
	assert.Equal(t, nil, err)
	defer instance.Uninit()
}
