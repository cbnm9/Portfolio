import java.util.Scanner;

public class MainMenu {
	Scanner sc=new Scanner(System.in);
	
	MainMenu() {}
	
	public int issueMenu()
	{
		System.out.println("/------Main Menu------/");
		System.out.println("  1. Add a student");
		System.out.println("  2. Show all students");
		System.out.println();
		System.out.println("  0. Exit");
		System.out.println("/---------------------/");
		System.out.print("Your choice > ");
		int sel=sc.nextInt();
		
		return sel;
	}
}
