//
// Created by Tan on 25-Oct-23.
//

#include "request.h"

HTTPClient http;

Request::Request() {

}

bool Request::ping(const String &host_name, const String &header, const String &header_value) {
    Serial.println(F("Checking API server connection: "));

    http.begin(host_name);
    http.addHeader(header, header_value);

    int httpCode = http.GET();

    if (httpCode > 0) {
        Serial.println(F("API server is reachable."));
        http.end();
        return true;
    } else {
        Serial.print(F("Failed to reach API server. Error code: "));
        Serial.println(httpCode);
        http.end();
        return false;
    }
}

http_response Request::get(const String &host_name, const String &path_name, const String &query, const String &header,
                           const String &header_value) {
    http_response _response;
    http.begin(host_name + path_name + query);
    http.addHeader(header, header_value);
    _response.status_code = http.GET();
    if (_response.status_code > 0) {
        if (_response.status_code == HTTP_CODE_OK) {
            _response.payload = http.getString();
            Serial.print(F("GET request is successful. Response: "));
            Serial.println(_response.payload);
        } else {
            _response.payload = "failed";
        }
    } else {
        _response.payload = "failed";
    }

    http.end();
    return _response;
}

http_response
Request::post(const String &host_name, const String &path_name, const String &payload, const String &header,
              const String &header_value) {
    http_response _response;
    http.begin(host_name + path_name);
    http.addHeader(header, header_value);
    http.addHeader("Content-Type", "application/xml");
    Serial.println(F("Sending payload: "));
    Serial.println(payload);
    _response.status_code = http.POST(payload);

    if (_response.status_code > 0) {
        if (_response.status_code == HTTP_CODE_OK) {
            _response.payload = http.getString();
            Serial.print(F("POST request is successful. Response: "));
            Serial.println(_response.payload);
        } else {
            _response.payload = "failed";
        }
    } else {
        _response.payload = "failed";
    }

    http.end();
    return _response;
}