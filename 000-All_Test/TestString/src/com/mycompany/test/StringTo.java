package com.mycompany.test;

import java.util.StringTokenizer;

//import java.util.StringTokenizer;
public class StringTo {

	public static void main(String[] args)
	{
		String text = "  We are Students   ";
		System.out.println("原字符串是： ");
		System.out.println(text);
		StringTokenizer st = new StringTokenizer(text, " ");
		System.out.println("st = " + st.toString());
		StringBuffer sb = new StringBuffer();
		int i = 1;
		while (st.hasMoreTokens())
		{
			i++;
			sb.append(st.nextToken());
			System.out.println("i = " + i);
		}
		System.out.print("去掉字符串中所有空格之后的字符串是：");
		System.out.println(sb.toString());
	}
}
