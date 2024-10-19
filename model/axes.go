package model

import "fmt"

type MovementExecutor interface {
	Execute(movement Vector, feed float32) error
}

type Axes struct {
	current_position Vector
	executor         MovementExecutor
}

func (i *Axes) Init(position Vector, executor MovementExecutor) {
	for _, dim := range []Dimension{X, Y, Z} {
		i.current_position[dim] = float32(0.0)
		coord, ok := position[dim]
		if ok {
			i.current_position[dim] = coord
		}
	}
	i.executor = executor
}

func (i *Axes) Move(movement Vector, feed float32) error {
	updated_position := Vector{}
	for _, dim := range []Dimension{X, Y, Z} {
		delta_coord, ok := movement[dim]
		if !ok {
			return fmt.Errorf("failed to retrieve one of required dimensions")
		}
		updated_position[dim] = i.current_position[dim] + delta_coord
	}
	err := i.executor.Execute(movement, feed)
	if err != nil {
		return err
	}
	i.current_position = updated_position
	return fmt.Errorf("NOT IMPLEMENTED")
}

func (i *Axes) Position() Vector {
	return i.current_position
}
