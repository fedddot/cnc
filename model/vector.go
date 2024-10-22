package model

import "math"

type Dimension uint

const (
	X Dimension = iota
	Y
	Z
)

type Coordinate interface {
	float32 | float64 | int
}

type Vector[T Coordinate] struct {
	coordinates map[Dimension]T
}

func (i *Vector[T]) Init(x, y, z T) {
	i.coordinates = map[Dimension]T{
		X: x,
		Y: y,
		Z: z,
	}
}

func (i Vector[T]) Get(dimension Dimension) T {
	return i.coordinates[dimension]
}

func (i Vector[T]) Add(other Vector[T]) Vector[T] {
	result := Vector[T]{}
	result.Init(
		i.Get(X)+other.Get(X),
		i.Get(Y)+other.Get(Y),
		i.Get(Z)+other.Get(Z),
	)
	return result
}

func (i Vector[T]) Equals(other Vector[T]) bool {
	for _, dim := range []Dimension{X, Y, Z} {
		if i.Get(dim) != other.Get(dim) {
			return false
		}
	}
	return true
}

func (i Vector[T]) Norm() T {
	sqr_norm := T(0)
	for _, dim := range []Dimension{X, Y, Z} {
		sqr_norm += i.coordinates[dim] * i.coordinates[dim]
	}
	return T(math.Sqrt(float64(sqr_norm)))
}
