package uk.ac.cam.ap801.spreadtest;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.util.LinkedList;
import java.util.List;
import java.util.Random;

import javax.swing.JFrame;
import javax.swing.JPanel;

public class Exporting extends JPanel{
	
	public List<Coordinates> allColoredPixels = new LinkedList<Coordinates>();
	public List<Coordinates> newlyColoredPixels = new LinkedList<Coordinates>();
	public List<Coordinates> nextColoredPixels = new LinkedList<Coordinates>();
	public BufferedImage canvas;
	public boolean[][] setMap;
	double spreadProb = 0.15;
	public static double colorChangeRate = 5;
	public boolean anyUpdates;

	
	public Exporting(int width, int height){
		canvas = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
		setMap = new boolean[width][height];
		for(int i = 0; i < height; i++){
			for(int j = 0; j < height; j++){
				setMap[i][j] = false;
			}		
		}
	}
		
	public Dimension getPreferredSize(){
		return new Dimension(canvas.getWidth(), canvas.getHeight());
	}
	
	public void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2 = (Graphics2D) g;
        g2.drawImage(canvas, null, null);
    }
	
	public void colorPixel(Coordinates c){
		canvas.setRGB(c.getX(), c.getY(), c.getColor().getRGB());
		repaint();
	}
	
	public static Color colorUpdate(Color c){
		Random randomGen = new Random();
		double green = c.getGreen();
		double red = c.getRed();
		double blue = c.getBlue();
		
		if (green > 255 - colorChangeRate){
			green -= randomGen.nextDouble() * colorChangeRate;
		}
		else{ 
			if (green < colorChangeRate){
				green += randomGen.nextDouble() * colorChangeRate;
			}
			
			else{ green -= (2 * randomGen.nextDouble() - 1) * colorChangeRate;
			}
		}
		
		if (blue > 255 - colorChangeRate){
			blue -= randomGen.nextDouble() * colorChangeRate;
		}
		else{ 
			if (blue < colorChangeRate){
				blue += randomGen.nextDouble() * colorChangeRate;
			}
			else{ blue -= (2 * randomGen.nextDouble() -1) * colorChangeRate;
			}
		}
		
		if (red > 255 - colorChangeRate){
			red -= randomGen.nextDouble() * colorChangeRate;
		}
		else{ 
			if (red < colorChangeRate){
				red += randomGen.nextDouble() * colorChangeRate;
			}
			else{ red -= (2 * randomGen.nextDouble() -1) * colorChangeRate;
			}
		}
		System.out.println("red: " + c.getRed() + "->" + (int)red + " green: " + c.getGreen() + "->" + (int)green + " blue: " + c.getBlue() + "->" + (int)blue );
		/*double walk = 0;
		for (int n=0;n<1000000;n++){
			walk += (2 * randomGen.nextDouble() - 1) * colorChangeRate;
		}
		System.out.println(walk/1000000);*/
		Color newColor = new Color((int)red, (int)green, (int)blue);
		return newColor;
	}
	
	public void updateNeighbours(Coordinates c, double prob){
		Random randomGen = new Random();
		
		for(int i = -1; i <= 1; i++){
			for(int j = -1; j <= 1; j++){
				if(!(c.getX()+i <= 0 || c.getX()+i>= canvas.getWidth() - 1|| c.getY()+j<= 0 || c.getY()+j>= canvas.getHeight() -1)){
					if (setMap[c.getX()+i][c.getY()+j] == false){
						if (randomGen.nextDouble() < prob){
							setMap[c.getX()+i][c.getY()+j] = true;
							Color nextColor = colorUpdate(c.getColor());
							nextColoredPixels.add(new Coordinates(c.getX() +i,c.getY()+j, nextColor));
							anyUpdates = true;
						}
					}
				}
			}
		}
	}
	
	public static void main(String[] args){
		JFrame frame = new JFrame("Spread demo");
		
		int width = 1000;
		int height = 1000;

        Exporting panel = new Exporting(width, height);

        frame.add(panel);
        frame.pack();
        frame.setVisible(true);
        frame.setResizable(false);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        panel.newlyColoredPixels.add(new Coordinates(width/2, height/2, Color.GRAY));
        boolean running = true;
        
        while(running){
        	panel.anyUpdates = false;
        	if (panel.newlyColoredPixels.size() == 0 ){
        		panel.spreadProb *= 1.05;
        		System.out.println("new Spread Probablility: " + panel.spreadProb);
        		panel.newlyColoredPixels.addAll(panel.allColoredPixels);
        		for(Coordinates c: panel.newlyColoredPixels){
            		panel.updateNeighbours(c, panel.spreadProb);
            		panel.colorPixel(c);
            		panel.repaint();
            	}
        		if(!(panel.anyUpdates)){ running = false;}
        	} else{ 
        		for(Coordinates c: panel.newlyColoredPixels){
        			panel.updateNeighbours(c, panel.spreadProb);
        			panel.colorPixel(c);
        			panel.repaint();
        		}
        		panel.allColoredPixels.addAll(panel.newlyColoredPixels);
        		panel.newlyColoredPixels.clear();
        		panel.newlyColoredPixels.addAll(panel.nextColoredPixels);
        		panel.nextColoredPixels.clear();
        	}
        } System.out.println("done");
	}  
}
