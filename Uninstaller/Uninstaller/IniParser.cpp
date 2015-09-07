
#include "stdafx.h"
#include "IniParser.h"
#include "IniFile.h"
#include "ServiceRegistry.h"


void IniParser::Parse(String Path)
{
	if (PathFileExists(Path.lpcwstr()))
	{

		IniFile ini;
		ini.Load(Path);


		IniSection*t = ini.FindIniSection("GLOBAL");
		if (t)
		{
			try
			{
				AppName = *t->GetKeyValue("AppName").Trim();
				AppVersion = *t->GetKeyValue("AppVersion").Trim();
				String s = *t->GetKeyValue("KeyInstallFolder").Trim();
				list<String> ls =s.SplitString("\t");
				if (ls.size() == 2)
				{
					s = *ls.front().Trim();
					ls.pop_front();
					ServiceRegistry::GetStringRegKey(s, *ls.front().Trim(), InstallDir, "");
				}
				else
				{
					Error = 2;
					return;
				}
			}
			catch (...)
			{
				Error = 2;
				return;
			}
		}


		t = ini.FindIniSection("UNINSTALL_TYPES");
		if (t)
		{
			try
			{
				//Ottengo tutti i file da copiare
				int i = 0;
				String temp;
				while ((temp = t->GetKeyValue(to_wstring(i))) != "")
				{
					UninstallType* ut=TipiDiDisistallazione.AddNewUninstallType(i, *temp.Trim());




					IniSection* tt = ini.FindIniSection("DELETE_FILE_"+to_wstring(i));
					if (tt)
					{
						try
						{
							int j = 0;
							String temp;
							while ((temp = tt->GetKeyValue(to_wstring(j))) != "")
							{
								ut->ListaFileDaCancellare.push_back(*temp.Trim());
								j++;
							}
						}
						catch (...)
						{
							Error = 4;
							return;
						}
					}

					tt = ini.FindIniSection("DELETE_FOLDER_" + to_wstring(i));
					if (tt)
					{
						try
						{
							int j = 0;
							String temp;
							while ((temp = tt->GetKeyValue(to_wstring(j))) != "")
							{
								ut->ListaCartelleDaCancellare.push_back(*temp.Trim());
								j++;
							}
						}
						catch (...)
						{
							Error = 5;
							return;
						}
					}


					tt = ini.FindIniSection("DELETE_REG_VALUE_" + to_wstring(i));
					if (tt)
					{
						try
						{
							int j = 0;
							String temp;
							while ((temp = tt->GetKeyValue(to_wstring(j))) != "")
							{
								list<String> st = temp.SplitString("\t");
								if (st.size() == 2)
								{
									String tt = st.front();
									st.pop_front();
									ut->ListaRegValueDaCancellare.push_back(make_tuple(tt, st.front()));
									j++;
								}
								else
								{
									Error = 6;
									return;
								}
								
							}
						}
						catch (...)
						{
							Error = 7;
							return;
						}
					}


					tt = ini.FindIniSection("DELETE_REG_KEY_" + to_wstring(i));
					if (tt)
					{
						try
						{
							int j = 0;
							String temp;
							while ((temp = tt->GetKeyValue(to_wstring(j))) != "")
							{
								list<String> st = temp.SplitString("\t");
								if (st.size() == 2)
								{
									String tt = st.front();
									st.pop_front();
									ut->ListaRegKeyDaCancellare.push_back(make_tuple(tt, st.front()));
									j++;
								}
								else
								{
									Error = 8;
									return;
								}

							}
						}
						catch (...)
						{
							Error = 9;
							return;
						}
					}




					i++;
				}

			}
			catch (...)
			{
				Error = 3;
				return;
			}
		}


		t = ini.FindIniSection("RUN_BEFORE_UNINSTALL");
		if (t)
		{
			try
			{
				int i = 0;
				String temp;
				while ((temp = t->GetKeyValue(to_wstring(i))) != "")
				{
					list<String> st = temp.SplitString("\t");
					if (st.size() == 2)
					{
						String t1 = *st.front().Trim();
						st.pop_front();
						TypeRun tr;
						if (t1 == "ASYNC")
							tr = Async;
						else if (t1 == "SYNC")
							tr = Sync;
						ListaProgrammiDaAvviarePrimaUninstall.push_back(make_tuple(tr, *st.front().Trim()));
						i++;
					}
					else
					{
						Error = 10;
						return;
					}

				}
			}
			catch (...)
			{
				Error = 11;
				return;
			}
		}



		t = ini.FindIniSection("KILL_PROCESS");
		if (t)
		{
			try
			{
				int i = 0;
				String temp;
				while ((temp = t->GetKeyValue(to_wstring(i))) != "")
				{
					ListaKillProcess.push_back(temp);
					i++;
				}
			}
			catch (...)
			{
				Error = 12;
				return;
			}
		}




		ini.RemoveAllSections();

	}
	else
		Error = 1;
}
