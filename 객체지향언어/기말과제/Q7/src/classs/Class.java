package classs;  //'class'라는 이름으로는 package가 만들어지지 않아서 'classs'로 만들었습니다
import subject.*;
import member.*;
import show.Showable;
import java.util.Scanner;

class Class implements Showable {
	private int id;
	private Subject subject;
	private Member[] mList;
	private int index=0;
	
	Scanner sc = new Scanner(System.in);
	
	Class () {}
	public Class(int id, SubjectMgr sm, MemberMgr mm)
	{
		mList=new Member[50];
		this.id=id;
		System.out.println(" -Class ID: " + id);
		addSubject(sm);
		addMember(mm);
	}
	
	public void addSubject(SubjectMgr sm)
	{
		sm.showAll();
		while(true)
		{
			System.out.print(" -Subject ID: ");
			int id=sc.nextInt();
			Subject s=sm.findSubject(id);
			if(s==null)
				System.out.println("==> Wrong ID");
			else
			{
				subject=s;
				break;
			}
		}
		
	}
	
	public void addMember(MemberMgr mm)
	{
		mm.showAllMember();
		while(true)
		{
			System.out.print("Member ID to add(0 to stop) > "); 
			int id=sc.nextInt();
			if(id==0)
				break;
			else
			{
				Member m=mm.findMember(id);
				if(m==null)
					System.out.println("==> Wrong ID");
				else
				{
					mList[index++]=m;
				}
			}
		}
	}
	
	public int getID() {return id;}
	public String getSubjectTitle() {return subject.getTitle();}
	public int getNumberOfMember() {return index;}
	
	public void showData()
	{
		for(int i=0; i<index; i++)
		{
			System.out.print(id + "\t");
			System.out.print(subject.getTitle() + "\t");
			if(mList[i] instanceof Student)
				System.out.print("Student\t\t");
			else
				System.out.print("Prof.\t\t");
			System.out.print(mList[i].getID() + "\t");
			System.out.print(mList[i].getName());
			System.out.println();
		}
	}
}
