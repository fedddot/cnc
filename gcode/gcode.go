package gcode

import (
	"cnc/client/manager"
	"cnc/client/model"
)

type CommandId string

const (
	G00 CommandId = "G00" // Rapid Positioning
	G01 CommandId = "G01" // Linear Interpolation

	G02 CommandId = "G02" // CW Circular Interpolation
	G03 CommandId = "G03" // CCW Circular Interpolation

	G28 CommandId = "G28" // Return Home

	G90 CommandId = "G90" // Toggle To Absolute Coordinates
	G91 CommandId = "G91" // Toggle To Relative Coordinates

	M00 CommandId = "M00" // Program stop
	M02 CommandId = "M02" // End of program
	M03 CommandId = "M03" // Spindle ON clockwise
	M04 CommandId = "M04" // Spindle ON counterclockwise
	M05 CommandId = "M05" // Spindle stop
	M06 CommandId = "M06" // Tool change
	M08 CommandId = "M08" // Flood colant ON
	M09 CommandId = "M09" // Flood colant OFF
	M30 CommandId = "M30" // End of program
)

type CircularMovementDirection int

const (
	CW CircularMovementDirection = iota
	CCW
)

type LocationMode int

const (
	ABSOLUTE LocationMode = iota
	RELATIVE
)

type MovementDescriptor struct {
	Target model.Vector[float32] `json:"target"`
	Feed   float32               `json:"feed"`
}

type CircularMovementDescriptor struct {
	MovementDescriptor
	RotationCenter model.Vector[float32]             `json:"rotation_center"`
	Direction      manager.CircularMovementDirection `json:"direction"`
}
