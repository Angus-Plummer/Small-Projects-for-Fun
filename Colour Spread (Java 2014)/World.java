package uk.ac.cam.ap801.spreadtest;

import java.awt.Color;
import java.util.LinkedList;
import java.util.List;

public class World {
	
	private int width;
	private int height;
	public boolean[][] pixelMap;
	public List<Coordinates> coloredPixels = new LinkedList<Coordinates>();
	public List<Coordinates> newlyColoredPixels = new LinkedList<Coordinates>();
	double spreadProb = 0.05;
	static int colorChangeRate = 10;
	boolean modifying = false;
	
	public World(int w,int h){
		width = w;
		height = h;
		pixelMap = new boolean[height][width];
		for(int i = 0; i < height; i++){
			for(int j = 0; j < height; j++){
				pixelMap[i][j] = false;
			}		
		}	
	}
	
	public int getWidth(){
		return width;
	}
	
	public int getHeight(){
		return height;
	}
	
	public void setPixel(int x, int y){
		if(!(x < 0 || x> width || y< 0 || y> height)){
			pixelMap[y][x] = true;
		}
	}
	
	public boolean getPixel(int x, int y){
		if(!(x < 0 || x>= width || y< 0 || y>= height)){
			return pixelMap[y][x];
		} else return true;
	}
}


