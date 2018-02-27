package uk.ac.cam.ap801.spreadtest;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics;

import javax.swing.JFrame;

public class Gui extends Canvas{
	
	public static World pixelMap;
	
	public void paint(Graphics g){
		super.paint(g);
		//synchronized(this);
		synchronized(pixelMap.coloredPixels){
			for(Coordinates c : pixelMap.coloredPixels){
				//pixelMap.updateNeighbours(c);
				g.setColor(c.getColor());
				g.drawLine(c.getX(), c.getY(), c.getX(), c.getY());
			}
		}
	}
	
	 public static void main(String[] args) {
		 
		 pixelMap = new World(400,400);
		 
    	 //pixelMap.coloredPixels.add(new Coordinates(0,0,Color.RED));
    	 pixelMap.coloredPixels.add(new Coordinates(399,399,Color.BLUE));
    	 pixelMap.setPixel(399, 399);
	     
		 JFrame frame = new JFrame();

	     frame.setSize(pixelMap.getWidth(), pixelMap.getHeight());
	     frame.add(new Gui());

	     frame.setVisible(true);
	     
	     while(pixelMap.coloredPixels.size() < pixelMap.getWidth() * pixelMap.getHeight()){
	    	
	    	synchronized(pixelMap.coloredPixels){
	    		for(Coordinates c: pixelMap.coloredPixels){
	    			pixelMap.updateNeighbours(c);
	    		}
	    	}
	    	
	    	synchronized(pixelMap.coloredPixels){
	    		pixelMap.coloredPixels.addAll(pixelMap.newlyColoredPixels);
	    	}
	    	pixelMap.newlyColoredPixels.clear();
	    	//System.out.println(pixelMap.coloredPixels.size());
	     }
	 }
}
