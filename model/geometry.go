package model

type Dimension uint

const (
	X Dimension = iota
	Y
	Z
)

type Vector struct {
	projections map[Dimension]float32
}

func (i *Vector) Init(x, y, z float32) {
	i.projections = map[Dimension]float32{
		X: x,
		Y: y,
		Z: z,
	}
}

func (i *Vector) Get(dimension Dimension) float32 {
	return i.projections[dimension]
}

func (i *Vector) Add(other Vector) Vector {
	result := Vector{}
	result.Init(
		i.Get(X)+other.Get(X),
		i.Get(Y)+other.Get(Y),
		i.Get(Z)+other.Get(Z),
	)
	return result
}

func (i *Vector) Equals(other Vector) bool {
	for _, dim := range []Dimension{X, Y, Z} {
		if i.Get(dim) != other.Get(dim) {
			return false
		}
	}
	return true
}
