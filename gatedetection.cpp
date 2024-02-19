#include <opencv2/opencv.hpp>

using namespace cv;

void detect_gate(const std::string& video_path, const cv::Scalar& lower_red, const cv::Scalar& upper_red)
{
    // Open the video file
    VideoCapture cap(video_path);

    // Check if the video file was opened successfully
    if (!cap.isOpened())
    {
        std::cout << "Error: Could not open video file." << std::endl;
        return;
    }

    // Loop over the frames in the video
    while (cap.isOpened())
    {
        // Read the next frame from the video
        Mat frame;
        cap >> frame;

        // Check if the frame was read successfully
        if (frame.empty())
        {
            break;
        }

        // Convert the frame to HSV color space
        Mat hsv_frame;
        cvtColor(frame, hsv_frame, COLOR_BGR2HSV);

        // Create a mask for the red color
        Mat mask;
        inRange(hsv_frame, lower_red, upper_red, mask);

        // Dilate and erode the mask to remove noise
        dilate(mask, mask, Mat(), Point(-1, -1), 1);
        erode(mask, mask, Mat(), Point(-1, -1), 1);

        // Detect the edges in the mask
        Mat edges;
        Canny(mask, edges, 50, 150);

        // Find the lines in the edges
        std::vector<Vec4i> lines;
        HoughLinesP(edges, lines, 1, CV_PI / 180, 50, 50, 5);

        // Draw the lines on the frame
        for (const Vec4i& line : lines)
        {
            cv::line(frame, Point(line[0], line[1]), Point(line[2], line[3]), Scalar(0, 255, 0), 2);
        }

        // Display the frame
        imshow("Gate Line Detection", frame);

        // Check if the user pressed the 'q' key to quit
        if (waitKey(30) & 0xFF == 'q')
        {
            break;
        }
    }

    // Close the video file
    cap.release();

    // Destroy all the windows
    destroyAllWindows();
}

int main(int argc, char** argv)
{
    // Get the path to the video file
    const std::string video_path = "/mnt/z/Programação/Huginn Team/gatevideo.mp4";

    // Get the lower and upper bounds of the red color
    const cv::Scalar lower_red = cv::Scalar(0, 75, 75);
    const cv::Scalar upper_red = cv::Scalar(10, 255, 255);

    // Detect the gate in the video
    detect_gate(video_path, lower_red, upper_red);

    return 0;
}