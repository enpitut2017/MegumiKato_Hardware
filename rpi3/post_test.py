import urllib.request, urllib.parse, urllib.error
import time

url = "http://192.168.11.143:3000/positions/"
latitude_num = 36.06216793281474
longitude_num = 140.0319304739485

class PostClass:
    def post_position(self, latitude_num, longitude_num):
        data = {
            "serial": "Kato Megumi no Macbook Pro",
            "latitude": latitude_num,
            "longitude": longitude_num
        }
        # ここでエンコードして文字→バイトにする！
        data = urllib.parse.urlencode(data).encode("utf-8")

        # with urllib.request.urlopen(url, timeout=2, data=data) as res:
        #     html = res.read().decode("utf-8")
        #     print(html)

        # POST処理
        try:
            with urllib.request.urlopen(url, timeout=2, data=data) as res:
                html = res.read().decode("utf-8")
                print(html)
        except:
            print("Error")

if __name__ == '__main__':
    p = PostClass()

    print("start")

    for i in range(100):
        latitude_num += 0.1
        longitude_num -= 0.1
        p.post_position(latitude_num, longitude_num)
        print(str(latitude_num) + "\t" + str(longitude_num))
        time.sleep(2)

    print("finish")
