package subject;

public class SubjectMgr {
	Subject[] sList;
	int index=0;
	
	SubjectMgr() {}
	public SubjectMgr(int n)
	{
		sList = new Subject[n];
	}
	
	public void addDefaultSubject()
	{
		sList[index++]=new Subject(index, "��ü���� ���");
		sList[index++]=new Subject(index, "������ ����");
		sList[index++]=new Subject(index, "�� ���α׷���");
		sList[index++]=new Subject(index, "JAVA ����");
	}
	
	public void showAll()
	{
		System.out.println("-----------------------");
		System.out.println("ID" + "\t" + "Title");
		System.out.println("-----------------------");
		for(int i=0; i<index; i++)
			sList[i].showData();
		System.out.println("-----------------------");
	}
	
	public void showSelect(String name)
	{
		Subject s=findSubject(name);
		
		if(s==null)
			System.out.println("==> Worng name");
		else
		{
			System.out.println("----------------------------");
			System.out.println("ID" + "\t" + "Title");
			System.out.println("----------------------------");
			s.showData();
			System.out.println("----------------------------");
		}
		
	}
	
	private Subject findSubject(String name)
	{
		for(int i=0; i<index; i++)
			if(sList[i].getTitle().compareTo(name)==0)
				return sList[i];
		return null;
		
	}
	
	public Subject findSubject(int id)
	{
		for(int i=0; i<index; i++)
			if(sList[i].getID()==id)
				return sList[i];
		return null;
	}
}
