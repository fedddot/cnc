package communication

type Request struct {
	Route  string
	Method string
	Body   map[string]interface{}
}

type Response struct {
	ResultCode int
	Body       map[string]interface{}
}

type Connection interface {
	RunRequest(request Request) (Response, error)
}
