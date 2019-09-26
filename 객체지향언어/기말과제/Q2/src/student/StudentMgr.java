package student;
import java.util.Scanner;

public class StudentMgr {
	Scanner sc=new Scanner(System.in);
	Student[] sList;
	int index=0;
	
	StudentMgr() {}
	public StudentMgr(int n)
	{
		sList=new Student[n];
	}
	
	public void addDefaultStudents()
	{
		sList[index++]=new Student(index, "Young J. Rho", "Computer Engineering");
		sList[index++]=new Student(index, "노영주", "컴퓨터공학");
		sList[index++]=new Student(index, "홍길동", "소프트웨어");
		sList[index++]=new Student(index, "Gidong Hong", "Software");
	}
	
	private int genID() {return index+1;}
	
	public void addStudent()
	{
		int id = genID();
		String name;
		String major;
		
		System.out.println("/----Adding a student----/");
		System.out.println(" - ID : " + id);
		System.out.print(" - Name: "); name=sc.nextLine();
		System.out.print(" - Major: "); major=sc.nextLine();
		System.out.println("--------------------------");
		
		sList[index++]=new Student(id, name, major);
		
		System.out.println("==> Succesfully added...");
		System.out.println("--------------------------");
	}
	
	public void showAll()
	{
		System.out.println("-----------------< All Student >-----------------");
		System.out.println("ID" + "\t" + "Name" + "\t\t" + "Major");
		System.out.println("-------------------------------------------------");
		for(int i=0; i<index; i++)
			sList[i].showData();
		System.out.println("-------------------------------------------------");
	}
	
}
