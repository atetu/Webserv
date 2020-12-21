#!/usr/bin/env python3

import os

print('Content-Type: text/html')
print('')
print('<html>')
print("""<style>
tr:nth-child(even) {
  background-color: #eee;
}
tr:nth-child(odd) {
  background-color: #fff;
}
th {
  color: white;
  background-color: black;
  width: 1hv;
}
</style>""")
print("""<table>
  <tr>
    <th>KEY</th>
    <th>VALUE</th>
  </tr>""")
for key, value in os.environ.items():
    print(f"""<tr>
    <td>{key}</td>
    <td>{value}</td>
  </tr>""")
print("</table>")
print('</html>')

import time
time.sleep(20)