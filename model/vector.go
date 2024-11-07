package model

import (
	"fmt"
	"math"
)

type Dimension int

const (
	X Dimension = iota
	Y
	Z
)

type FloatCoordinate float32

func (i FloatCoordinate) String() string {
	return fmt.Sprintf("%.*f", 3, i)
}

type Coordinate interface {
	FloatCoordinate | float32 | float64 | int
}

type Vector[T Coordinate] struct {
	X T `json:"x"`
	Y T `json:"y"`
	Z T `json:"z"`
}

func (i Vector[T]) Add(other Vector[T]) Vector[T] {
	return Vector[T]{
		X: i.X + other.X,
		Y: i.Y + other.Y,
		Z: i.Z + other.Z,
	}
}

func (i Vector[T]) Equals(other Vector[T]) bool {
	return (i.X == other.X) && (i.Y == other.Y) && (i.Z == other.Z)
}

func (i Vector[T]) Norm() T {
	return T(math.Sqrt(float64(i.X*i.X) + float64(i.Y*i.Y) + float64(i.Z*i.Z)))
}

func (i Vector[T]) Negate() Vector[T] {
	return Vector[T]{
		X: -i.X,
		Y: -i.Y,
		Z: -i.Z,
	}
}

func (i Vector[T]) Get(dim Dimension) (T, error) {
	switch dim {
	case X:
		return i.X, nil
	case Y:
		return i.Y, nil
	case Z:
		return i.Z, nil
	default:
		return 0, fmt.Errorf("invalid dimension received: %d", dim)
	}
}

func (i *Vector[T]) Set(dim Dimension, value T) error {
	switch dim {
	case X:
		i.X = value
	case Y:
		i.Y = value
	case Z:
		i.Z = value
	default:
		return fmt.Errorf("invalid dimension received: %d", dim)
	}
	return nil
}
