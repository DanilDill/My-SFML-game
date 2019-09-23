
//////////////////получение миссии////////////
int getCurrentMission(int health) {
	int mission = 0;
	if (health == 100) { mission = 0; }
	if(health == 40) {mission = 1;}
	if (health == 20) { mission = 2; }
	return mission;
}

std::string getTextMision(int CurrentMission) {
	std::string TextMission="";
	switch (CurrentMission)
	{
	case 0:TextMission = "У вас все хорошо"; break;
	case 1:TextMission = "Будь внимательнее"; break;
	case 2:TextMission = "еще раз траванешься - помрешь"; break;

	}
	return TextMission;
}
