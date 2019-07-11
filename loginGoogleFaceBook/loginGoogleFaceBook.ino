//from https://create.arduino.cc/projecthub/phpoc_man/arduino-login-into-google-and-facebook-9ae91d?f=1
//change to import user id's and passwords from a file excluded by git config
//credit for excluding passwords from git respo
//https://arduino.stackexchange.com/questions/40411/hiding-wlan-password-when-pushing-to-github/55583

#include "config.h"

//#include <Phpoc.h> //not required on esp32, already has wifi shield
#include "WiFi.h"
#include "ESPAsyncWebServer.h"


// Replace your GOOGLE_CLIENT_ID and GOOGLE_CLIENT_SECRET here

//String GOOGLE_CLIENT_ID      = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx.apps.googleusercontent.com";
//String GOOGLE_CLIENT_SECRET  = "";

//https://developers.facebook.com/apps/436911710482618/fb-login/settings/
//iotdemo1
String FACEBOOK_APP_ID ="xxxxxxxxxxxxxxxxxxxx"; //
String FACEBOOK_CLIENT_TOKEN = "xxxxxxxxxxxxxxxxxxxxxxxxx";

String gg_access_token                  = "";
String gg_refresh_token                 = "";
unsigned long gg_access_token_expire_at = 0;

String fb_access_token                  = "";
unsigned long fb_access_token_expire_at = 0;

//PhpocServer websocket_server(80);
AsyncWebServer server(80);

String base64_decode(const String &encoded_string)
{
	String decoded_string = "";
	static const String base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	for(int pointer = 0; pointer < encoded_string.length(); pointer++) {
		char cur_char = encoded_string.charAt(pointer);
		if(cur_char == '=')
			break;

		int index = base64_chars.indexOf(cur_char);
		char_array_4[i++] = index;
		if (i == 4) {
			char_array_3[0] = ((char_array_4[0] << 2) & 0xFC) | ((char_array_4[1] >> 4) & 0x03);
			char_array_3[1] = ((char_array_4[1] << 4) & 0xF0) | ((char_array_4[2] >> 2) & 0x0F);
			char_array_3[2] = ((char_array_4[2] << 6) & 0xC0) | ((char_array_4[3] >> 0) & 0x3F);

			for (j = 0; (j < 3); j++)
				decoded_string += (char)char_array_3[j];

			i = 0;
		}
	}

	if(i == 2)
	{
		char last_char = (char)((char_array_4[0] << 2) & 0xFC) | ((char_array_4[1] >> 4) & 0x03);
		decoded_string += (char)last_char;
	}
	else
	if(i == 3)
	{
		char last_char_1 = ((char_array_4[0] << 2) & 0xFC) | ((char_array_4[1] >> 4) & 0x03);
		char last_char_2 = ((char_array_4[1] << 4) & 0xF0) | ((char_array_4[2] >> 2) & 0x0F);
		decoded_string += (char)last_char_1;
		decoded_string += (char)last_char_2;
	}
	else
	if(i == 1)
		Serial.println("base64_decode: invalid base64_endcode data");

	return decoded_string;
}

String JSONvalue(const String &json_str, const String &key)
{
	String search_phrase = String('"');
	search_phrase.concat(key);
	search_phrase.concat('"');

	int key_pos = json_str.indexOf(search_phrase);

	if(key_pos != -1) {
		int start_pos = json_str.indexOf(":", key_pos);
		if(start_pos != -1) {
			int end_pos = json_str.indexOf(",", start_pos);

			if(end_pos == -1) {
				end_pos = json_str.indexOf("}", start_pos);
			}

			if(end_pos != -1) {
				String value;

				value = json_str.substring(start_pos + 1, end_pos);
				value.trim();
				if((char)value.charAt(0) == '"')
					value.remove(0, 1);

				if((char)value.charAt(value.length() - 1) == '"')
					value.remove(value.length() - 1, 1);

				return value;
			}
			else
			{
				//Serial.print(F("JSONvalue: key \""));
				//Serial.print(key);
				//Serial.println(F("\" invalid JSON format"));
			}
		}
		else
		{
			//Serial.print(F("JSONvalue: key \""));
			//Serial.print(key);
			//Serial.println(F("\" invalid JSON format"));
		}
	}
	else
	{
		//Serial.print(F("JSONvalue: key \""));
		//Serial.print(key);
		//Serial.println(F("\" NOT found"));
	}

	return String("");
}

String http_resp_header(PhpocClient &client){
	String header = "";
	while(1){
		if(client.available()){
			String line = client.readLine();

			if(line == "\r\n")
				break;
			else
				header += line;
		}

		if(!client.connected()){
			client.stop();
			break;
		}
	}

	return header;
}

String http_resp_body(PhpocClient &client){
	String body = "";
	while(1){
		if(client.available()){
			 char c = client.read();
			 body += c;
		}

		if(!client.connected()){
			client.stop();
			break;
		}
	}

	return body;
}

String https(const String &method, const String &url, String &body)
{
	PhpocClient client;
	String http_host;
	String http_path;
	char host[100];

	int pos = url.indexOf('/');

	if(pos == -1)
	{
		Serial.print(F("https: Invalid URL"));
		return "";
	}

	http_host = url.substring(0, pos);
	http_path = url.substring(pos);
	http_host.toCharArray(host, http_host.length() + 1);

	if(client.connectSSL(host, 443)){
		//Serial.print(F("https: Connected to "));
		//Serial.println(url);

		client.println(method + " " + http_path + " HTTP/1.1");
		client.println("Host: " + http_host);
		client.println(F("Connection: close"));
		client.println(F("Accept: */*"));
		client.println(F("Content-Type: application/x-www-form-urlencoded"));
		client.print(F("Content-Length: ")); client.println(body.length());
		client.println();

		client.print(body);

		http_resp_header(client);
		//String response_header = http_resp_header(client);
		String response_body = http_resp_body(client);
		//Serial.println(response_header);
		//Serial.println(response_body);

		return response_body;
	}
	else
	{
		Serial.print(F("https: CANNOT Connected to "));
		Serial.println(url);
		return "";
	}
}

void websocket_send(const String &msg)
{
	websocket_server.write(msg.c_str(), msg.length());
}

void googleDeviceOAuthLogin(){
	String device_code      = "";
	String user_code        = "";
	String verification_url = "";
	long expires_in          = 0;
	int interval            = 0;
	bool is_valid = true;

	Serial.println();
	Serial.println(F("================ LOGIN TO GOOGLE ================"));

	// Step 1: Request device and user codes
	Serial.println(F("Step 1: Request device and user codes"));


	{ // put code in braces to save memory by using local variables
		request_body = F("client_id=");
		request_body += GOOGLE_CLIENT_ID;
		request_body += F("&scope=profile email");

		String response_body = https("POST", "accounts.google.com/o/oauth2/device/code", request_body);
		Serial.println(response_body);

		// Step 2: Handle the authorization server response
		Serial.println(F("Step 2: Handle the authorization server response"));

		device_code      = JSONvalue(response_body, "device_code");
		user_code        = JSONvalue(response_body, "user_code");
		expires_in       = JSONvalue(response_body, "expires_in").toInt();
		interval         = JSONvalue(response_body, "interval").toInt();
		verification_url = JSONvalue(response_body, "verification_url");
	}


	if(!device_code.equals(""))
	{
		// Step 3: Display the user code
		Serial.println(F("Step 3: Display the user code"));
		Serial.println(F("NEXT"));
		Serial.print(F(" + visit "));
		Serial.print(verification_url);
		Serial.println(F(" on your desktop or smartphone"));
		Serial.print(F(" + enter this code: "));
		Serial.println(user_code);

		{ // put code in braces to save memory by using local variables
			String msg;

			msg  = "{\"provider\": \"google\",";
			msg += "\"action\": \"LOGIN\",";
			msg += "\"verification_url\": \"" + verification_url + "\",";
			msg += "\"user_code\": \"" + user_code + "\"}";
			websocket_send(msg);
		}

		// Step 4: Poll authorization server
		Serial.println(F("Step 4: Poll authorization server"));
		int poll_max = expires_in / interval;
		long token_expires_in = 0;
		String gg_id_token;

		for(int poll_count = 0; poll_count < poll_max; poll_count++){
			String request_body = F("client_id=");
			request_body += GOOGLE_CLIENT_ID;
			request_body += F("&client_secret=");
			request_body += GOOGLE_CLIENT_SECRET;
			request_body += F("&code=");
			request_body += device_code;
			request_body += F("&grant_type=http://oauth.net/grant_type/device/1.0");

			String response_body = https("POST", "www.googleapis.com/oauth2/v4/token", request_body);

			gg_access_token  = JSONvalue(response_body, "access_token");
			gg_refresh_token = JSONvalue(response_body, "refresh_token");
			gg_id_token      = JSONvalue(response_body, "id_token");
			token_expires_in = JSONvalue(response_body, "expires_in").toInt();

			if(!gg_access_token.equals("")) // process outside to save memory: local variables (response_body and request_body) are free. These variables are very big in size.
				break;

			delay(interval * 1000);
		}

		if(!gg_access_token.equals(""))
		{
			gg_access_token_expire_at = millis() + token_expires_in * 1000;

			Serial.println();
			Serial.print(F("GOOGLE ACCESS_TOKEN: "));
			Serial.println(gg_access_token);
			//Serial.print(F("GOOGLE ID_TOKEN: "));
			//Serial.println(gg_id_token);

			// Step 5: get User Profile
			/*
			NOTE: for Google, We don't need to make new request to get User Profile from server
			because the User Profile has been included in id_token in JWT format
			*/
			int dot_pos_1 = gg_id_token.indexOf('.');
			int dot_pos_2 = gg_id_token.lastIndexOf('.');
			String decoded_string;

			{ // put code in braces to save memory by using local variables
				String jwt_body = gg_id_token.substring(dot_pos_1 + 1, dot_pos_2);
				decoded_string =  base64_decode(jwt_body);
			}

			String oauth_uid = JSONvalue(decoded_string, "sub");
			String name      = JSONvalue(decoded_string, "name");
			String email     = JSONvalue(decoded_string, "email");

			if(!oauth_uid.equals(""))
			{
				// send success message to web
				String msg  = "{\"provider\": \"google\",";
				msg += "\"action\": \"SUCCESS\",";
				msg += "\"name\": \"" + name + "\",";
				msg += "\"email\": \"" + email + "\"}";
				websocket_send(msg);

				Serial.println(F("LOGGED IN WITH GOOGLE ACCOUNT: "));
				Serial.print(F(" + name: "));
				Serial.println(name);
				Serial.print(F(" + email: "));
				Serial.println(email);
			}
		}
	}
	//else
		//Serial.println(F("Invalid resonse from Google"));
}

void facebookDeviceOAuthLogin(){
	String device_code      = "";
	String user_code        = "";
	String verification_url = "";
	long expires_in         = 0;
	int interval            = 0;

	Serial.println();
	Serial.println(F("=============== LOGIN TO FACEBOOK ==============="));

	// Step 1: Request device and user codes
	Serial.println(F("Step 1: Request device and user codes"));

	{ // put code in braces to save memory by using local variables
		String request_body = "access_token=" + FACEBOOK_APP_ID + "|" + FACEBOOK_CLIENT_TOKEN;
		request_body += "&scope=email";

		String response_body = https("POST", "graph.facebook.com/v2.6/device/login", request_body);
    Serial.print("response_body:");
		Serial.println(response_body);

		// Step 2: Handle the authorization server response
		Serial.println(F("Step 2: Handle the authorization server response"));

		device_code      = JSONvalue(response_body, "code");
    Serial.print("device_code:");
    Serial.println(device_code);
		user_code        = JSONvalue(response_body, "user_code");
    Serial.print("user_code:");
    Serial.println(user_code);
		expires_in       = JSONvalue(response_body, "expires_in").toInt();
    Serial.print("expires_in:");
    Serial.println(expires_in);
		interval         = JSONvalue(response_body, "interval").toInt();
    Serial.print("interval:");
    Serial.println(interval);
		verification_url = JSONvalue(response_body, "verification_uri");
    Serial.print("verification_url:");
    Serial.println(verification_url);
	}

	if(!device_code.equals(""))
	{
    Serial.println("device_code.equals('') is false");
		// Step 3: Display the user code
		Serial.println(F("Step 3: Display the user code"));
		Serial.println(F("NEXT"));
		Serial.print(F(" + visit "));
		Serial.print(verification_url);
		Serial.println(F(" on your desktop or smartphone"));
		Serial.print(F(" + enter this code: "));
		Serial.println(user_code);


		{ // put code in braces to save memory by using local variables
			String msg  = "{\"provider\": \"facebook\",";
			msg += "\"action\": \"LOGIN\",";
			msg += "\"verification_url\": \"" + verification_url + "\",";
			msg += "\"user_code\": \"" + user_code + "\"}";
			websocket_send(msg);
		}

		// Step 4: Poll authorization server
		Serial.println(F("Step 4: Poll authorization server"));
		int poll_max = expires_in / interval;
		long token_expires_in = 0;

		for(int poll_count = 0; poll_count < poll_max; poll_count++){
			String request_body = "access_token=" + FACEBOOK_APP_ID + "|" + FACEBOOK_CLIENT_TOKEN;
			request_body += "&code=" + device_code;

			String response_body = https("POST", "graph.facebook.com/v2.6/device/login_status", request_body);
			//Serial.println(response_body);

			fb_access_token  = JSONvalue(response_body, "access_token");
			token_expires_in = JSONvalue(response_body, "expires_in").toInt();

			if(!fb_access_token.equals("")) // process outside to save memory: local variables (response_body and request_body) are free. These variables are very big in size.
				break;

			delay(interval * 1000);
		}

		if(!fb_access_token.equals(""))
		{
			fb_access_token_expire_at = millis() + token_expires_in * 1000;
			Serial.println();
			Serial.print(F("FACEBOOK ACCESS_TOKEN: "));
			Serial.println(fb_access_token);

			String oauth_uid;
			String name;
			String email;

			// Step 5: get User Profile
			//Serial.println(F("Step 5: get User Profile"));

			{ // put code in braces to save memory by using local variables
				String url = "";
				url += F("graph.facebook.com/v3.2/me");
				url += F("?fields=id,name,email,picture");
				url += F("&access_token=");
				url += fb_access_token;

				String request_body = "";
				String response_body = https("GET", url, request_body);
				//Serial.println(response_body);

				oauth_uid = JSONvalue(response_body, "id");
				name      = JSONvalue(response_body, "name");
				email     = JSONvalue(response_body, "email");
			}

			if(!oauth_uid.equals(""))
			{
				// send success message to web
				String msg  = "{\"provider\": \"facebook\",";
				msg += "\"action\": \"SUCCESS\",";
				msg += "\"name\": \"" + name + "\",";
				msg += "\"email\": \"" + email + "\"}";
				websocket_send(msg);

				Serial.println(F("LOGGED IN WITH FACEBOOK ACCOUNT: "));
				Serial.print(F(" + name: "));
				Serial.println(name);
				Serial.print(F(" + email: "));
				Serial.println(email);
			}
		}
	}
	else
		Serial.println(F("Invalid repsonse from Facebook"));
}

void setup(){
	Serial.begin(115200);
	while(!Serial)
		;

	//Phpoc.begin(PF_LOG_SPI | PF_LOG_NET);
	Phpoc.begin();
	websocket_server.beginWebSocket("login");
	Serial.print(F("WebSocket server address : "));
	Serial.println(Phpoc.localIP());
}

void loop(){
	PhpocClient client = websocket_server.available();

	if (client) {
		String ws_str = client.readLine();

		if(ws_str == "google\r\n")
			googleDeviceOAuthLogin();
		else
		if(ws_str == "facebook\r\n")
			facebookDeviceOAuthLogin();
	}
}
