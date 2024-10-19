package model

type Dimension uint

const (
	X Dimension = iota
	Y
	Z
)

type Vector map[Dimension]float32
