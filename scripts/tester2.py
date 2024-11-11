import asyncio
import aiohttp

async def send_get_request(session, url):
	async with session.get(url) as response:
		print(f"Status Code for {url}: {response.status}")

async def main():
	url = "http://localhost:8080"

	num_requests = 100

	async with aiohttp.ClientSession() as session:
		tasks = []
		for _ in range(num_requests):
			task = send_get_request(session, url)
			tasks.append(task)
		await asyncio.gather(*tasks)

if __name__ == "__main__":
	asyncio.run(main())
