package communication

type TestConnection struct {
	Connection
	action func(request Request) (Response, error)
}

func (i *TestConnection) RunRequest(request Request) (Response, error) {
	return i.action(request)
}

func (i *TestConnection) Init(action func(request Request) (Response, error)) {
	i.action = action
}
