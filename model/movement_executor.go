package model

type MovementExecutor interface {
	Execute(movement Vector, feed float32) error
}
