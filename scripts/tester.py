import requests

def print_response(response, method, url):
	yellow = "\033[33m"
	reset = "\033[0m"

	print(f"{yellow}{method} Request to URL: {url}{reset}\n")

	print(f"Status Code: {response.status_code} - {response.reason}\n")
	print(f"Request URL: {response.url}\n")

	print(f"HTTP-Headers:")
	for header, value in response.headers.items():
		print(f"{header}: {value}")

	print(f"\nResponse Content:")
	print(response.text[:100])

def main():
	print("42 Webserv Tester\n")

	# GET
	url = "http://localhost:8080"
	response = requests.get(url)
	print_response(response, "GET", url)

	# POST form data
	url = "http://localhost:8080/send_message_de.html"
	data = {"name": "42", "message": "Hello World!"}
	response = requests.post(url, data=data)
	print_response(response, "POST", url)

	# POST file uplaod
	url = "http://localhost:8080/uploads/"
	file = "README.md"
	with open(file, 'rb') as f:
		files = {'file': f}
		response = requests.post(url, files=files)
	print_response(response, "POST", url)

	# DELETE
	url = "http://localhost:8080/uploads/delete.me"
	response = requests.delete(url)
	print_response(response, "DELETE", url)

if __name__ == "__main__":
	main()
