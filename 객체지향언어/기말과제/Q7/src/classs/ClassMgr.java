package classs;  //'class'라는 이름으로는 package가 만들어지지 않아서 'classs'로 만들었습니다
import subject.*;
import member.*;
import java.util.Scanner;

public class ClassMgr {
	Class[] cList;
	int index=0;
	Scanner sc=new Scanner(System.in);
	
	ClassMgr() {}
	public ClassMgr(int n)
	{
		cList=new Class[n];
	}
	
	public void addClass(SubjectMgr sm, MemberMgr mm)
	{
		System.out.println("------< Adding a class >-------");
		cList[index++]=new Class(index, sm, mm);
		System.out.println("-------------------------------");
	}
	
	private void showClassList()
	{
		System.out.println("--< Class List in short >---------------------");
		System.out.println("ID" + "\t" + "Subject" + "\t\t" + "Number of members");
		System.out.println("----------------------------------------------");
		for(int i=0; i<index; i++)
			System.out.println(cList[i].getID() + "\t" + cList[i].getSubjectTitle() 
								+ "\t" + cList[i].getNumberOfMember());
		System.out.println("----------------------------------------------");
	}
	
	public void showClass()
	{
		if(index==0)
			System.out.println("==No Class==");
		else
		{
			Class c;
			showClassList();
			while(true)
			{
				System.out.print(" - Class ID ? > ");
				int id=sc.nextInt();
				c=findClass(id);
				if(c==null)
					System.out.println("==> Wrong ID...");
				else
					break;
			}
			System.out.println("--< Class Info >-------------------------------------------");
			System.out.println("ID" + "\t" + "Subject" + "\t\t" + "Member Type"
								+ "\t" + "ID" + "\t" + "Name");
			System.out.println("-----------------------------------------------------------");
			c.showData();
			System.out.println("-----------------------------------------------------------");
		}
	}
	
	public void showAllClass()
	{
		if(index==0)
			System.out.println("== No Class ==");
		else
		{
			System.out.println("--< Class List in full >------------------------------------");
			System.out.println("ID" + "\t" + "Subject" + "\t\t" + "Member Type"
								+ "\t" + "ID" + "\t" + "Name");
			System.out.println("------------------------------------------------------------");
			for(int i=0; i<index; i++)
				cList[i].showData();
			System.out.println("------------------------------------------------------------");
		}
		
	}
	
	private Class findClass(int id)
	{
		for(int i=0; i<index; i++)
			if(cList[i].getID()==id)
				return cList[i];
		return null;
	}
}
