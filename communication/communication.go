package communication

type Request struct {
	Route  string
	Method string
	Body   interface{}
}

type Response struct {
	ResultCode int
	Body       interface{}
}

type Connection interface {
	RunRequest(request Request) (Response, error)
}
