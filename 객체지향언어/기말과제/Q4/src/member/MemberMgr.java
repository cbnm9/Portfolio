package member;
import java.util.Scanner;

public class MemberMgr {
	Scanner sc=new Scanner(System.in);
	Member[] mList;
	int index=0;
	
	public MemberMgr() {}
	public MemberMgr(int n)
	{
		mList=new Member[n];		
	}
	
	public void addDefaultMember()
	{
		mList[index++]=new Student(index, "Casey Jones", "Computer Engineering");
		mList[index++]=new Student(index, "캐시 존스", "컴퓨터공학");
		mList[index++]=new Student(index, "전우치", "소프트웨어");
		mList[index++]=new Student(index, "Woochi Jeon", "Software");
		mList[index++]=new Professor(index, "Young J. Rho", "Computer Eng School");
		mList[index++]=new Professor(index, "노영주", "컴퓨터공학부");
		mList[index++]=new Professor(index, "홍길동", "소프트웨어전공");
		mList[index++]=new Professor(index, "Gildong Hong", "Software");
	}
	
	private int genID() {return index+1;}
	
	public void addStudent()
	{
		int id = genID();
		String name;
		String major;
		
		System.out.println("/----Adding a student----/");
		System.out.println(" - ID: " + id);
		System.out.print(" - Name: "); name=sc.nextLine();
		System.out.print(" - Major: "); major=sc.nextLine();
		System.out.println("--------------------------");
		
		mList[index++]=new Student(id, name, major);
		
		System.out.println("==> Succesfully added...");
		System.out.println("--------------------------");
	}
	
	public void addProfessor()
	{
		int id = genID();
		String name;
		String department;
		
		System.out.println("/----Adding a professor----/");
		System.out.println(" - ID: " + id);
		System.out.print(" - Name: "); name=sc.nextLine();
		System.out.print(" - Department: "); department=sc.nextLine();
		System.out.println("----------------------------");
		
		mList[index++]=new Professor(id, name, department);
		
		System.out.println("==> Succesfully added...");
		System.out.println("----------------------------");
	}
	
	public void showAllStudent()
	{
		System.out.println("-----------------< All Student >-----------------");
		System.out.println("ID" + "\t" + "Name" + "\t\t" + "Major");
		System.out.println("-------------------------------------------------");
		for(int i=0; i<index; i++)
		{
			if(mList[i] instanceof Student)
				mList[i].showData();
		}
		System.out.println("-------------------------------------------------");
	}
	
	public void showAllProfessor()
	{
		System.out.println("----------------< All Professor >----------------");
		System.out.println("ID" + "\t" + "Name" + "\t\t" + "Department");
		System.out.println("-------------------------------------------------");
		for(int i=0; i<index; i++)
		{
			if(mList[i] instanceof Professor)
				mList[i].showData();
		}
		System.out.println("-------------------------------------------------");
	}
	
	public void showAllMember()
	{
		System.out.println("------------------< All Member >----------------------");
		System.out.println("Group" + "\t" + "ID" + "\t" + "Name" + "\t\t" + "Major/Department");
		System.out.println("------------------------------------------------------");
		for(int i=0; i<index; i++)
		{
			if(mList[i] instanceof Student)
				System.out.print("Student\t");
			else
				System.out.print("Prof.\t");
			mList[i].showData();
		}
		System.out.println("-----------------------------------------------------");
	}
}
