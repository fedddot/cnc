package communication

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

var s_test_host_addr string = "http://127.0.0.1"
var s_test_service string = "5000"

func TestInitRunRequest(t *testing.T) {
	// GIVEN:
	connection := HttpConnection{}
	post_request := map[string]interface{}{
		"id": "motor1",
		"a0": 10,
		"a1": 11,
		"b0": 12,
		"b1": 13,
		"en": 14,
	}
	route := "steppers"

	// WHEN
	connection.Init(s_test_host_addr, s_test_service)
	response, err := connection.RunRequest(
		Request{
			Method: "POST",
			Route:  route,
			Body:   post_request,
		},
	)
	assert.Equal(t, nil, err)
	assert.Equal(t, 0, response.ResultCode)
	response, err = connection.RunRequest(
		Request{
			Method: "GET",
			Route:  route,
			Body:   nil,
		},
	)
	assert.Equal(t, nil, err)
	assert.Equal(t, 0, response.ResultCode)
}
