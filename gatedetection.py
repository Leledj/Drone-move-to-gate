import cv2
import numpy as np

def detect_gate(video_path, lower_red, upper_red):

    cap = cv2.VideoCapture(video_path)

    if not cap.isOpened():
        print("Error: Could not open video.")
        return

    while cap.isOpened():
        ret, frame = cap.read()

        if not ret:
            break

        hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        mask = cv2.inRange(hsv_frame, lower_red, upper_red)
        kernel = np.ones((5, 5), np.uint8)
        mask = cv2.dilate(mask, kernel, iterations=1)
        mask = cv2.erode(mask, kernel, iterations=1)
        
        edges = cv2.Canny(mask, threshold1=50, threshold2=150)
        
        lines = cv2.HoughLinesP(edges, 1, np.pi/180, threshold=50, minLineLength=50, maxLineGap=5)

        if lines is not None:
            for line in lines:
                x1, y1, x2, y2 = line[0]
                cv2.line(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                
                center_x = (x1 + x2) // 2
                center_y = (y1 + y2) // 2
                cv2.circle(frame, (center_x, center_y), 5, (0, 0, 255), -1)
                
                frame_center_y = frame.shape[0] // 2
                
                if center_y < frame_center_y:  # Check if the line is above the center of the frame
                    print("Move upward")
                elif center_y >= frame_center_y:  # Check if the line is at or below the center of the frame
                    print("Move forward")

        cv2.imshow('Gate Line Detection', frame)
        if cv2.waitKey(30) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    video_path = '/mnt/z/Programação/Huginn Team/gatevideo.mp4'
    lower_red = np.array([0, 75, 75])
    upper_red = np.array([10, 255, 255])
    detect_gate(video_path, lower_red, upper_red)
