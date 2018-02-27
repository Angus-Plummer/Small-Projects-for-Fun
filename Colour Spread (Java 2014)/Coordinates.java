package uk.ac.cam.ap801.spreadtest;

import java.awt.Color;

public class Coordinates {
	private int x;
	private int y;
	private Color c;
	
	public Coordinates(int setx,int sety, Color setc){
		x=setx;
		y=sety;
		c = setc;
	}

	public int getX(){
		return x;
	}
	
	public int getY(){
		return y;
	}
	
	public Color getColor(){
		return c;
	}
	
	public void setColor(Color color){
		c = color ;
	}

}


