package test;

import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.Rect;
import org.opencv.core.Size;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.Imgproc;

public class TestBlur {

	public static void main(String[] args) {
		//TestAllBlur();
		//TestRectBlur();
		TestDiagBlur();
	}

	// 全图模糊
	public static void TestAllBlur() {
		System.loadLibrary(Core.NATIVE_LIBRARY_NAME);

		Mat src = Imgcodecs.imread("src.jpg");
		Mat dst = new Mat();
		Imgproc.GaussianBlur(src, dst, new Size(51, 51), 0);

		Imgcodecs.imwrite("dst.jpg", dst);
	}

	// 局部矩形模糊
	public static void TestRectBlur() {
		System.loadLibrary(Core.NATIVE_LIBRARY_NAME);

		Mat src = Imgcodecs.imread("src.jpg");
		//底部的1/6模糊
		Rect rect = new Rect(0, src.height() * 5 / 6, src.width(), src.height() / 6);
		// System.out.println(rect.toString());
		Mat dst = src.submat(rect);
		Imgproc.GaussianBlur(dst, dst, new Size(51, 51), 0);

		Imgcodecs.imwrite("dst.jpg", src);
	}

	// 斜角模糊
	public static void TestDiagBlur() {
		System.loadLibrary(Core.NATIVE_LIBRARY_NAME);

		Mat src = Imgcodecs.imread("src.jpg");
		//底部的2/6斜斜到底部的1/6
		double _h = (double)src.height()/(6.0* src.width());//高度的步进值
		for (int i = 0; i < src.width(); i++) {
			//宽度为1，高度递减的矩形
			Rect rect = new Rect(i, (int) (2.0*src.height()/3.0 + i * _h), 1, (int) (src.height()/3.0 - i * _h));
			// System.out.println(rect.toString());
			Mat dst = src.submat(rect);
			Imgproc.GaussianBlur(dst, dst, new Size(51, 51), 0);
		}
		Imgcodecs.imwrite("dst.jpg", src);
	}
}
