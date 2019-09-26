import member.MemberMgr;
import subject.SubjectMgr;

public class MainCtrl {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		MainMenu menu = new MainMenu();
		MemberMgr mm = new MemberMgr(50);
		SubjectMgr sm = new SubjectMgr(50);
		
		mm.addDefaultMember();
		sm.addDefaultSubject();
		
		while(true)
		{
			int sel=menu.issueMenu();
			
			switch(sel)
			{
			case 1:
				mm.addStudent();
				break;
			case 2:
				mm.addProfessor();
				break;
			case 3:
				mm.showAllStudent();
				break;
			case 4:
				mm.showAllProfessor();
				break;
			case 5:
				mm.showAllMember();
				break;
			case 6:
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
