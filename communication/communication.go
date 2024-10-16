package communication

type Request struct {
	method string
	body   map[string]interface{}
}

type Response struct {
	result_code int
	body        map[string]interface{}
}

type Connection interface {
	RunRequest(request Request) (Response, error)
}
