package model

type Axes struct {
	current_position Vector
	executor         MovementExecutor
}

func (i *Axes) Init(position Vector, executor MovementExecutor) {
	i.current_position = position
	i.executor = executor
}

func (i *Axes) Move(movement Vector, feed float32) error {
	err := i.executor.Execute(movement, feed)
	if err != nil {
		return err
	}
	i.current_position = i.current_position.Add(movement)
	return nil
}

func (i *Axes) Position() Vector {
	return i.current_position
}
