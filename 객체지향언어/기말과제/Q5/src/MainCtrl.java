import subject.SubjectMgr;
import java.util.Scanner;

public class MainCtrl {
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Scanner sc=new Scanner(System.in);
		SubjectMgr sm = new SubjectMgr(50);
		sm.addDefaultSubject();		
		
		sm.showAll();
		while(true)
		{
			System.out.println();
			System.out.print(" - Subject name? > ");	
			String name=sc.nextLine();
			sm.showSelect(name);
		}
	}

}
