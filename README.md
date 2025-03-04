# Maixdock-Based Driver Drowsiness Detection Using CNN
Ai camera and Maixdock controller to 
detect the driver drowsiness by using Machine Learning. In addition to it we use 
GPS to track the location and alcohol sensor to detect the drunk and drive scenario. 
We use heart beat sensor to find the driver health.Esp8266 is connected with IoT to 
update the status from the sensors.

## Overview
This project implements a **driver drowsiness detection system** using a **Convolutional Neural Network (CNN)** on the **Sipeed Maixdock (K210)**. The system processes real-time facial images to determine whether a driver is drowsy and triggers an alert when necessary.

## Features
✅ Real-time drowsiness detection using **CNN**  
✅ Runs on **Sipeed Maixdock (K210 RISC-V AI)** for edge computing  
✅ Low power consumption and high efficiency  
✅ Audio/visual alert system upon detecting drowsiness  

## Technologies Used
- **Hardware:** Sipeed Maixdock K210  
- **Software:** Python, MaixPy, OpenCV  
- **Machine Learning:** TensorFlow/Keras (for model training)  

## Installation & Setup

1️⃣ Install Dependencies
Ensure you have Python installed, then install required libraries:
pip install numpy opencv-python maixpy
2️⃣ Load the Model to Maixdock

Convert the trained CNN model into KPU format for K210:

maix_convert --model drowsiness_model.h5 --output drowsiness_model.kmodel

Upload drowsiness_model.kmodel to the Maixdock board.

3️⃣ Run the Detection Script
python drowsiness_detection.py

How It Works:
Captures real-time video feed from the camera.
Uses CNN to classify the driver’s eye state (open/closed).
If the eyes remain closed for a defined duration, an alarm is triggered.

Results
Achieved 87% accuracy on test dataset.
Detects drowsiness with minimal latency on edge devices.

Future Improvements:

🔹 Improve accuracy with more diverse datasets
🔹 Optimize CNN model for faster inference on Maixdock
🔹 Add support for additional fatigue indicators (head pose, yawning)
