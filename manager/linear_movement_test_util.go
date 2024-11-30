package manager

import (
	"cnc/client/communication"
	"cnc/client/hardware"
	"fmt"
)

func get_mappings(axis AxisTag) (hardware.StepperMotorControlOutputs, hardware.StepperMotorDirectionOutputs, error) {
	switch axis {
	case X:
		return hardware.StepperMotorControlOutputs{
				hardware.ENA: 15,
				hardware.ENB: 20,
			},
			hardware.StepperMotorDirectionOutputs{
				hardware.ATop: 17,
				hardware.ABtm: 16,
				hardware.BTop: 18,
				hardware.BBtm: 19,
			},
			nil
	case Y:
		return hardware.StepperMotorControlOutputs{
				hardware.ENA: 9,
				hardware.ENB: 14,
			},
			hardware.StepperMotorDirectionOutputs{
				hardware.ATop: 10,
				hardware.ABtm: 11,
				hardware.BTop: 12,
				hardware.BBtm: 13,
			},
			nil
	case Z:
		return hardware.StepperMotorControlOutputs{
				hardware.ENA: 3,
				hardware.ENB: 8,
			},
			hardware.StepperMotorDirectionOutputs{
				hardware.ATop: 4,
				hardware.ABtm: 5,
				hardware.BTop: 6,
				hardware.BBtm: 7,
			},
			nil
	}
	return nil, nil, fmt.Errorf("unsupported axis tag: %s", axis)
}

func InitMotors(mapping MotorsMapping, connection communication.Connection) (map[AxisTag]hardware.StepperMotor, error) {
	result := make(map[AxisTag]hardware.StepperMotor, 0)
	for tag, id := range mapping {
		controls, directions, err := get_mappings(tag)
		if err != nil {
			return result, err
		}
		motor := hardware.StepperMotor{}
		err = motor.Init(
			hardware.StepperMotorId(id),
			hardware.StepperMotorConfig{
				ControlOutputs:   controls,
				DirectionOutputs: directions,
			},
			connection,
		)
		if err != nil {
			return result, err
		}
		result[tag] = motor
	}
	return result, nil
}

func UninitMotors(motors map[AxisTag]hardware.StepperMotor) error {
	for _, motor := range motors {
		err := motor.Uninit()
		if err != nil {
			return err
		}
	}
	return nil
}
