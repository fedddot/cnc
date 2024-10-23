package model

import "math"

type Coordinate interface {
	float32 | float64 | int
}

type Vector[T Coordinate] struct {
	X, Y, Z T
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
