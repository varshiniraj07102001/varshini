# generated by maixhub, tested on maixpy v0.6.0_2_g9720594
# copy files to TF card and plug into board and power on

import sensor, image, lcd, time
import sensor,image,lcd,time
from machine import UART
from board import board_info
from fpioa_manager import fm
import KPU as kpu
import json
import gc, sys

fm.register(board_info.LED_R, fm.fpioa.UART1_TX, force=True)
uart_1 = UART(UART.UART1,9600,8,0,0, timeout=1000, read_buf_len=4096)

input_size = (224, 224)
labels = ['Seat empty', 'Normal', 'yawn', 'eyes close', 'drowsiness symtoms']

def lcd_show_except(e):
    import uio
    err_str = uio.StringIO()
    sys.print_exception(e, err_str)
    err_str = err_str.getvalue()
    img = image.Image(size=input_size)
    img.draw_string(0, 10, err_str, scale=1, color=(0xff,0x00,0x00))
    lcd.display(img)

def main(labels = None, model_addr="/sd/m.kmodel", sensor_window=input_size, lcd_rotation=2, sensor_hmirror=False, sensor_vflip=False):
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_windowing(sensor_window)
    sensor.set_hmirror(sensor_hmirror)
    sensor.set_vflip(sensor_vflip)
    sensor.run(1)

    lcd.init(type=1)
    lcd.rotation(lcd_rotation)
    lcd.clear(lcd.WHITE)

    if not labels:
        with open('labels.txt','r') as f:
            exec(f.read())
    if not labels:
        print("no labels.txt")
        img = image.Image(size=(320, 240))
        img.draw_string(90, 110, "no labels.txt", color=(255, 0, 0), scale=2)
        lcd.display(img)
        return 1
    try:
        img = image.Image("startup.jpg")
        lcd.display(img)
    except Exception:
        img = image.Image(size=(320, 240))
        img.draw_string(90, 110, "loading model...", color=(255, 255, 255), scale=2)
        lcd.display(img)

    try:
        task = None
        task = kpu.load(model_addr)
        while(True):
            img = sensor.snapshot()
            t = time.ticks_ms()
            fmap = kpu.forward(task, img)
            t = time.ticks_ms() - t
            plist=fmap[:]
            pmax=max(plist)
            max_index=plist.index(pmax)
            img.draw_string(0,0, "%.2f : %s" %(pmax, labels[max_index].strip()), scale=2, color=(255, 0, 0))
            img.draw_string(0, 200, "t:%dms" %(t), scale=2, color=(255, 0, 0))
            lcd.display(img)
            # print(labels[max_index].strip())


            if labels[max_index].strip()=="Seat empty":
               uart_1.write('a')
               time.sleep_ms(1000)
               print(labels[max_index].strip())

            elif labels[max_index].strip()=="Normal":
                 uart_1.write('b')
                 time.sleep_ms(1000)
                 print(labels[max_index].strip())

            elif labels[max_index].strip()=="yawn":
                 uart_1.write('c')
                 time.sleep_ms(1000)
                 print(labels[max_index].strip())

            elif labels[max_index].strip()=="eyes close":
                 uart_1.write('d')
                 time.sleep_ms(1000)
                 print(labels[max_index].strip())

            elif labels[max_index].strip()=="drowsiness symtoms":
                 uart_1.write('e')
                 time.sleep_ms(1000)
                 print(labels[max_index].strip())

    except Exception as e:
        raise e
    finally:
        if not task is None:
            kpu.deinit(task)


if __name__ == "__main__":
    try:
         main(labels=labels, model_addr=0x300000)
        #main(labels=labels, model_addr="/sd/model-29141.kmodel")
    except Exception as e:
        sys.print_exception(e)
        lcd_show_except(e)
    finally:
        gc.collect()
