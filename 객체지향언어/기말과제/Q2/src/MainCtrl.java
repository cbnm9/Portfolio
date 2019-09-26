import student.*;

public class MainCtrl {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		MainMenu menu = new MainMenu();
		StudentMgr sm=new StudentMgr(50);
		sm.addDefaultStudents();
		
		while(true)
		{
			int sel=menu.issueMenu();
			
			switch(sel)
			{
			case 1:
				sm.addStudent();
				break;
			case 2:
				sm.showAll();
				break;
			case 0:
				return;
			default:
				System.out.println("==> Wrong choice...");
				break;
			}
		}
	}

}
