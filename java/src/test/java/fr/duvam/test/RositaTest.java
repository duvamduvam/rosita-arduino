package fr.duvam.test;

import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.Scanner;

import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;

import org.junit.Test;

import fr.duvam.arduino.test.ArduinoComm;
import jssc.SerialPort;
import jssc.SerialPortException;

public class RositaTest {

	@Test
	public void testGetSoundLenght() {

		/*
		 * JFrame frame = new JFrame();
		 * frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); frame.setLocation(100,
		 * 100); frame.setSize(1200, 800); frame.setVisible(true);
		 * //frame.startPlaying();
		 * 
		 * EmbeddedMediaPlayerComponent mediaPlayerComponent = new
		 * EmbeddedMediaPlayerComponent() {
		 * 
		 * @Override protected FullScreenStrategy onGetFullScreenStrategy() { return new
		 * XFullScreenStrategy(frame); } };
		 * 
		 * EmbeddedMediaPlayer mediaPlayer = mediaPlayerComponent.getMediaPlayer();
		 * 
		 * mediaPlayer.playMedia(music);
		 */

		String music = "/home/david/Nextcloud/InstantUpload/Son/artiste.wav";

		// AudioMediaPlayerComponent mediaPlayerComponent = new
		// AudioMediaPlayerComponent(); // <--- 1
		// mediaPlayerComponent.getMediaPlayer().playMedia(music); // <--- 2

		// mediaPlayerComponent.

		/*
		 * try {
		 * 
		 * AudioInputStream audioInputStream = AudioSystem.getAudioInputStream(new
		 * File(music)); Clip clip = AudioSystem.getClip(); clip.open(audioInputStream);
		 * clip.start(); clip.loop(Clip.LOOP_CONTINUOUSLY); } catch (Exception ex) {
		 * ex.printStackTrace(); }
		 */

	}

	// @Test
	public void readAndRemove() throws IOException {

		String keyFile = "/home/david/Nextcloud/rosita/java/src/test/resources/keys.txt";

		// ClassLoader classLoader = new RositaTest().getClass().getClassLoader();
		File file = new File(keyFile);

		Scanner s = new Scanner(file);
		while (s.hasNext()) {
			System.out.println("line : " + s.next());
		}
		s.close();

		// delete content
		System.out.println("line : " + file.getPath());
		if (file.exists()) {
			file.delete();
			try {
				file.createNewFile();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	// @Test
	public void testGIF() throws MalformedURLException {

		// URL url =
		// RositaTest.class.getResource("/home/david/Nextcloud/rosita/java/videos/gifs/test.gif");
		URL url = Paths.get("/home/david/Nextcloud/rosita/java/videos/gifs/test.gif").toUri().toURL();
		ImageIcon imageIcon = new ImageIcon(url);
		JLabel label = new JLabel(imageIcon);

		JFrame f = new JFrame("Animation");
		f.getContentPane().add(label);
		f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		f.pack();
		f.setLocationRelativeTo(null);
		f.setVisible(true);
	}

	@Test
	public void testArduinoCom() throws Exception {
		ArduinoComm arduino = new ArduinoComm();

		LinkedList<String> inputs = 	
				new LinkedList<String>(Arrays.asList("a", "b", "c", "aaa", "bbb"));

		//arduino.openConnection();
		for (String input: inputs) {
			//String input = "12345";
			arduino.sendString(input);
			System.out.println(input);
			Thread.sleep(5000);
		}
		//arduino.closeConnection();

	}
	
	@Test
	public void sendArduinoMsg() {
        SerialPort serialport = new SerialPort("/dev/ttyACM0");
        try{
            serialport.openPort();
            serialport.setParams(9600, 8, 1, 0);
            serialport.writeString("a");
            serialport.closePort();

        }
        catch(SerialPortException ex){
            System.err.println(ex);
        }
	}

}
