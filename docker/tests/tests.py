import unittest
import os

import requests

BASE_URL = "http://127.0.0.1:80"


class WebservTestCase(unittest.TestCase):

	def assertGetStatusCode(self, url, status_code):
		response = requests.get(BASE_URL + url)
		
		self.assertTrue(response.status_code == status_code, "GET {} expected {} but got {}".format(url, status_code, response.status_code))

	def assertGet(self, url, content):
		response = requests.get(BASE_URL + url)
		
		self.assertTrue(response.text == content, "GET {} expected {} but got {}".format(url, content, response.text))

	def assertPostStatusCode(self, url, status_code, body):
		response = requests.post(BASE_URL + url, data=body)
		
		self.assertTrue(response.status_code == status_code, "POST {} expected {} but got {}".format(url, status_code, response.status_code))
	
	# def assertPostStatusCodeChunked(self, url, status_code, body):
	# 	header = {'Transfer-encoding': 'chunked'}
	# 	response = requests.post(BASE_URL + url, headers=header, data=body)
		
	# 	self.assertTrue(response.status_code == status_code, "POST {} expected {} but got {}".format(url, status_code, response.status_code))

	def assertDeleteStatusCode(self, url, status_code):
		response = requests.delete(BASE_URL + url)
		
		self.assertTrue(response.status_code == status_code, "DELETE {} expected {} but got {}".format(url, status_code, response.status_code))
		
	def assertPutStatusCode(self, url, status_code, body):
		response = requests.put(BASE_URL + url, data=body)
		
		self.assertTrue(response.status_code == status_code, "PUT {} expected {} but got {}".format(url, status_code, response.status_code))

	def assertOptionStatusCode(self, url, status_code):
		response = requests.options(BASE_URL + url)
		
		self.assertTrue(response.status_code == status_code, "OPTIONS {} expected {} but got {}".format(url, status_code, response.status_code))

	def assertHeadStatusCode(self, url, status_code):
		response = requests.head(BASE_URL + url)
		
		self.assertTrue(response.status_code == status_code, "HEAD {} expected {} but got {}".format(url, status_code, response.status_code))

class TestGetMethod(WebservTestCase):
	
	def test_simple(self):
		self.assertGetStatusCode("/", 200)

	def test_not_found(self):
		self.assertGetStatusCode("/hello", 404)

	def test_no_listing(self):
		self.assertGetStatusCode("/no-listing", 404)


class TestPostMethod(WebservTestCase):
	
	def test_simple_file(self):
		self.assertPostStatusCode("/test__simple_file.txt", 201, "Hello World".encode("ascii"))
		self.assertGet("/test__simple_file.txt", "Hello World")
	
	def test_simple_file_but_no_duplicate(self):
		self.assertPostStatusCode("/test__simple_file_2.txt", 201, "Hello World".encode("ascii"))
		self.assertPostStatusCode("/test__simple_file_2.txt", 200, "Hello World".encode("ascii"))
		self.assertGet("/test__simple_file_2.txt", "Hello World")
	def test_simple_file_post_sh(self):
		self.assertPostStatusCode("/_tests/x.sh", 405, "Not autorised".encode("ascii"))

class TestPutMethod(WebservTestCase):

	def test_simple_file_put(self):
		self.assertPutStatusCode("/test__simple_file_put.txt", 201, "Filed changed".encode("ascii"))
		self.assertGet("/test__simple_file_put.txt", "Filed changed")
	
	def test_simple_file_but_double_changes_put(self):
		self.assertPutStatusCode("/test__simple_file_put_2.txt", 201, "Filed changed".encode("ascii"))
		self.assertPutStatusCode("/test__simple_file_put_2.txt", 200, "Filed changed 2 times".encode("ascii"))
		self.assertGet("/test__simple_file_put_2.txt", "Filed changed 2 times")

class TestDeleteMethod(WebservTestCase):
	
	def test_simple_file_delete(self):
		self.assertPostStatusCode("/test__will_be_deleted.txt", 201, "Hello World".encode("ascii"))
		self.assertDeleteStatusCode("/test__will_be_deleted.txt", 200)
	
	def test_not_found(self):
		self.assertDeleteStatusCode("/test__cannot_be_deleted_because_not_exists.txt", 204)

class TestOptionsMethod(WebservTestCase):
	
	def test_simple_file_options_base(self):
		self.assertOptionStatusCode("/", 200)
	def test_simple_file_options_tests(self):
		self.assertOptionStatusCode("/_tests", 200)
	def test_simple_file_options_path(self):
		self.assertOptionStatusCode("/wrong_path", 200)

class TestHeadMethod(WebservTestCase):
	
	def test_simple_file_head_base(self):
		self.assertHeadStatusCode("/", 200)
	def test_simple_file_head_path(self):
		self.assertHeadStatusCode("/wrong_path", 404)
	def test_simple_file_head_tests(self):
		self.assertHeadStatusCode("/_tests", 404)


if __name__ == '__main__':

	def remove_if_exists(path):
		if os.path.exists(path):
			os.remove(path)
	
	remove_if_exists("test__simple_file.txt")
	remove_if_exists("test__simple_file_2.txt")
	remove_if_exists("test__simple_file.txt")
	remove_if_exists("test__simple_file_put.txt")
	remove_if_exists("test__simple_file_put_2.txt")
	remove_if_exists("test__will_be_deleted.txt")
	remove_if_exists("test__cannot_be_deleted_because_not_exists.txt")
	
	unittest.main()
