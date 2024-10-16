package hardware

type Request struct {
	method string
	body   map[string]interface{}
}

type Response struct {
	result_code int
	body        map[string]interface{}
}

type StepperMotor struct {
	a0, a1, b0, b1, en int
	id                 string
	request_performer
}

func (i *StepperMotor) Init(a0, a1, b0, b1, en int, id string) error {
	i.a0 = a0
	i.a1 = a1
	i.b0 = b0
	i.b1 = b1
	i.en = en
	i.id = id

}
