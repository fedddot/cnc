package model

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

type TestExecutor struct {
	action func(movement Vector, feed float32) error
}

func (i *TestExecutor) Init(action func(movement Vector, feed float32) error) {
	i.action = action
}

func (i *TestExecutor) Execute(movement Vector, feed float32) error {
	return i.action(movement, feed)
}

func TestAxes(t *testing.T) {
	// GIVEN
	test_movement := Vector{}
	test_movement.Init(10.8, 14.98, 39.1)
	test_feed := float32(394240.724)

	// WHEN
	executor := TestExecutor{}
	executor.Init(
		func(movement Vector, feed float32) error {
			return nil
		},
	)

	test_axes := Axes{}
	test_axes.Init(test_movement, &executor)

	// THEN
	err := test_axes.Move(test_movement, test_feed)
	assert.Equal(t, nil, err)
}
