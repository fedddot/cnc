package communication

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
)

type HttpConnection struct {
	server_hostname    string
	server_servicename string
}

func (i *HttpConnection) Init(server_hostname string, server_servicename string) {
	i.server_hostname = server_hostname
	i.server_servicename = server_servicename
}

func (i *HttpConnection) runGetRequest(path string) (Response, error) {
	response := Response{}
	http_response, err := http.Get(path)
	if err != nil {
		return response, err
	}
	defer http_response.Body.Close()
	body, err := io.ReadAll(http_response.Body)
	if err != nil {
		return response, err
	}
	var body_parsed interface{}
	err = json.Unmarshal(body, &body_parsed)
	if err != nil {
		return response, err
	}
	response.Body = body_parsed
	response.ResultCode = http_response.StatusCode
	return Response{}, nil
}

func (i *HttpConnection) runDeleteRequest(path string) (Response, error) {
	response := Response{}
	request, err := http.NewRequest("DELETE", path, nil)
	if err != nil {
		return response, err
	}
	http_response, err := http.DefaultClient.Do(request)
	if err != nil {
		return response, err
	}
	defer http_response.Body.Close()
	body, err := io.ReadAll(http_response.Body)
	if err != nil {
		return response, err
	}
	var body_parsed interface{}
	err = json.Unmarshal(body, &body_parsed)
	if err != nil {
		return response, err
	}
	response.Body = body_parsed
	response.ResultCode = http_response.StatusCode
	return Response{}, nil
}

func (i *HttpConnection) runPostRequest(path string, body interface{}) (Response, error) {
	response := Response{500, nil}
	body_bytes, err := json.Marshal(body)
	if err != nil {
		return response, err
	}
	http_response, err := http.Post(path, "application/json", bytes.NewBuffer(body_bytes))
	if err != nil {
		return response, err
	}
	defer http_response.Body.Close()
	http_response_body_bytes, err := io.ReadAll(http_response.Body)
	if err != nil {
		return response, err
	}
	var body_parsed interface{}
	err = json.Unmarshal(http_response_body_bytes, &body_parsed)
	if err != nil {
		return response, err
	}
	response.Body = body_parsed
	response.ResultCode = http_response.StatusCode
	return Response{}, nil
}

func (i *HttpConnection) runPutRequest(path string, body interface{}) (Response, error) {
	response := Response{}
	body_bytes, err := json.Marshal(body)
	if err != nil {
		return response, err
	}
	request, err := http.NewRequest("PUT", path, bytes.NewBuffer(body_bytes))
	if err != nil {
		return response, err
	}
	http_response, err := http.DefaultClient.Do(request)
	if err != nil {
		return response, err
	}
	defer http_response.Body.Close()
	http_response_body_bytes, err := io.ReadAll(http_response.Body)
	if err != nil {
		return response, err
	}
	var body_parsed interface{}
	err = json.Unmarshal(http_response_body_bytes, &body_parsed)
	if err != nil {
		return response, err
	}
	response.Body = body_parsed
	response.ResultCode = http_response.StatusCode
	return Response{}, nil
}

func (i *HttpConnection) RunRequest(request Request) (Response, error) {
	path := fmt.Sprintf("%s:%s/%s", i.server_hostname, i.server_servicename, request.Route)
	switch request.Method {
	case "GET":
		return i.runGetRequest(path)
	case "POST":
		return i.runPostRequest(path, request.Body)
	case "DELETE":
		return i.runDeleteRequest(path)
	case "PUT":
		return i.runPutRequest(path, request.Body)
	default:
		return Response{500, nil}, fmt.Errorf("unsupported method received: %s", request.Method)
	}
}
