#pragma once
#include "stdafx.h"
#include "IniParser.h"
#include "IniFile.h"
#include "ServiceRegistry.h"

IniParser::IniParser() {}
IniParser::~IniParser() {}

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
				AppName = t->GetKeyValue("AppName");
				AppName.Trim();
				//Ottengo la versione da installare e la key nel registro che dovrebbe contenere quella corrente
				NewVersion = t->GetKeyValue("InstallVersion");
				String RegKey = t->GetKeyValue("GetCurrentVersion");
				list<String> s = RegKey.SplitString("\t");
				if (s.size() == 2)
				{
					RegKey = s.front();
					s.pop_front();
					String out;
					ServiceRegistry::GetStringRegKey(RegKey, s.front(), out, "");
					OldVersion = out;
				}
				else
				{
					Error = 2;
					return;
				}
			}
			catch (...)
			{
				Error = 3;
				return;
			}
		}


		t = ini.FindIniSection("UNINSTALL_COMMAND");
		if (t)
		{
			try
			{
				//Ottengo la folder di installazione corrente
				String RegKey = t->GetKeyValue("GetInstallFolder");
				list<String> s = RegKey.SplitString("\t");
				if (s.size() == 2)
				{
					RegKey = s.front();
					s.pop_front();
					String out;
					ServiceRegistry::GetStringRegKey(RegKey, s.front(), out, "");
					OldInstallFolder = out;
				}
				else
				{
					Error = 4;
					return;
				}
				UninstallCommand = t->GetKeyValue("UninstallCommand");
				UninstallArgument = t->GetKeyValue("UninstallArgument");
				String temp = t->GetKeyValue("UninstallBeforeInstall");
				UninstallBeforeInstall = *temp.Trim() == "1";

			}
			catch (...)
			{
				Error = 5;
				return;
			}
		}



		t = ini.FindIniSection("COPY_FILE");
		if (t)
		{
			try
			{
				//Ottengo tutti i file da copiare
				int i = 0;
				String temp;
				while ((temp = t->GetKeyValue(to_wstring(i))) != "")
				{
					list<String> st = temp.SplitString("\t");
					if (st.size() == 2)
					{
						String tt = st.front();
						st.pop_front();
						ListaFileDaCopiare.push_back(make_tuple(tt, st.front()));
						i++;
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


		t = ini.FindIniSection("DELETE_FILE");
		if (t)
		{
			try
			{
				//Ottengo tutti i file da eliminare
				int i = 0;
				String temp;
				while ((temp = t->GetKeyValue(to_wstring(i))) != "")
				{
					ListaFileDaCancellare.push_back(temp);
					i++;
				}
			}
			catch (...)
			{
				Error = 8;
				return;
			}
		}


		t = ini.FindIniSection("CREATE_REG_KEY");
		if (t)
		{
			try
			{
				//Ottengo tutti i file da eliminare
				int i = 0;
				String temp;
				while ((temp = t->GetKeyValue(to_wstring(i))) != "")
				{
					list<String> st = temp.SplitString("\t");
					if (st.size() == 2)
					{
						String tt = st.front();
						st.pop_front();
						ListaRegKeyDaCreare.push_back(make_tuple(tt, st.front()));
						i++;
					}
					else
					{
						Error = 9;
						return;
					}
				}
			}
			catch (...)
			{
				Error = 10;
				return;
			}
		}


		t = ini.FindIniSection("SET_REG_VALUE");
		if (t)
		{
			try
			{
				//Ottengo tutti i file da eliminare
				int i = 0;
				String temp;
				while ((temp = t->GetKeyValue(to_wstring(i))) != "")
				{
					list<String> st = temp.SplitString("\t");
					if (st.size() == 4)
					{
						String t1 = st.front();
						st.pop_front();
						String t2 = st.front();
						st.pop_front();
						String t3 = st.front();
						st.pop_front();
						ListaRegValueDaSettare.push_back(make_tuple(t1, t2, t3, st.front()));
						i++;
					}
					else
					{
						Error = 11;
						return;
					}

				}
			}
			catch (...)
			{
				Error = 12;
				return;
			}
		}


		t = ini.FindIniSection("DELETE_REG_KEY");
		if (t)
		{
			try
			{
				//Ottengo tutti i file da eliminare
				int i = 0;
				String temp;
				while ((temp = t->GetKeyValue(to_wstring(i))) != "")
				{
					list<String> st = temp.SplitString("\t");
					if (st.size() == 2)
					{
						String t1 = st.front();
						st.pop_front();
						ListaRegKeyDaCancellare.push_back(make_tuple(t1, st.front()));
						i++;
					}
					else
					{
						Error = 13;
						return;
					}

				}
			}
			catch (...)
			{
				Error = 14;
				return;
			}
		}


		t = ini.FindIniSection("DELETE_REG_VALUE");
		if (t)
		{
			try
			{
				//Ottengo tutti i file da eliminare
				int i = 0;
				String temp;
				while ((temp = t->GetKeyValue(to_wstring(i))) != _T(""))
				{
					list<String> st = temp.SplitString("\t");
					if (st.size() == 2)
					{
						String t1 = st.front();
						st.pop_front();
						ListaRegValueDaCancellare.push_back(make_tuple(t1, st.front()));
						i++;
					}
					else
					{
						Error = 15;
						return;
					}


				}
			}
			catch (...)
			{
				Error = 16;
				return;
			}
		}



		t = ini.FindIniSection("RUN_AFTER_COMPLETE");
		if (t)
		{
			try
			{
				//Ottengo tutti i file da eliminare
				int i = 0;
				String temp;
				while ((temp = t->GetKeyValue(to_wstring(i))) != "")
				{
					list<String> st = temp.SplitString("\t");
					if (st.size() == 2)
					{
						String t1 = st.front();
						t1.Trim();
						st.pop_front();
						TypeRun tr;
						if (t1 == "ASYNC")
							tr = Async;
						else if (t1 == "SYNC")
							tr = Sync;

						String t = st.front();
						ListaProgrammiDaAvviareDopoInstallazione.push_back(make_tuple(tr, *t.Trim()));
						i++;
					}
					else
					{
						Error = 17;
						return;
					}

				}
			}
			catch (...)
			{
				Error = 18;
				return;
			}
		}



		t = ini.FindIniSection("RUN_AFTER_ABORT");
		if (t)
		{
			try
			{
				//Ottengo tutti i file da eliminare
				int i = 0;
				String temp;
				while ((temp = t->GetKeyValue(to_wstring(i))) != "")
				{
					list<String> st = temp.SplitString("\t");
					if (st.size() == 2)
					{
						String t1 = st.front();
						t1.Trim();
						st.pop_front();
						TypeRun tr;
						if (t1 == "ASYNC")
							tr = Async;
						else if (t1 == "SYNC")
							tr = Sync;

						String t = st.front();
						ListaProgrammiDaAvviareDopoAbort.push_back(make_tuple(tr, *t.Trim()));
						i++;
					}
					else
					{
						Error = 19;
						return;
					}

				}
			}
			catch (...)
			{
				Error = 20;
				return;
			}
		}


		t = ini.FindIniSection("LINKS");
		if (t)
		{
			try
			{
				String dl = t->GetKeyValue("DesktopLink");
				list<String> st = dl.SplitString("\t");
				if (st.size() == 3)
				{
					String k = st.front();
					k.Trim();
					st.pop_front();
					String v = st.front();
					v.Trim();
					st.pop_front();

					String str = st.front();
					int d = StrToIntW(str.Trim()->lpcwstr());
					st.pop_front();

					unsigned long out;
					ServiceRegistry::GetDWORDRegKey(k, v,out, d);

					DesktopLink = (int)out;
				}
				else
				{
					Error = 21;
					return;
				}

				dl = t->GetKeyValue("StartMenuLink");
				st = dl.SplitString("\t");
				if (st.size() == 3)
				{
					String k = st.front();
					k.Trim();
					st.pop_front();
					String v = st.front();
					v.Trim();
					st.pop_front();

					String str = st.front();
					DWORD d = StrToIntW(str.Trim()->lpcwstr());
					st.pop_front();

					DWORD out;
					ServiceRegistry::GetDWORDRegKey(k, v, out, d);
					StartMenuLink = out;
				}
				else
				{
					Error = 22;
					return;
				}


				String sml = t->GetKeyValue("StartMenuLink");

			}
			catch (...)
			{
				Error = 23;
				return;
			}
		}


		t = ini.FindIniSection("START_MENU_LINK");
		if (t)
		{
			try
			{
				StartMenuFolderName = t->GetKeyValue("FolderName");

				int i = 0;
				String temp;
				while ((temp = t->GetKeyValue(to_wstring(i))) != "")
				{
					list<String> st = temp.SplitString("\t");
					if (st.size() == 2)
					{
						String k = st.front();
						k.Trim();
						st.pop_front();
						StartMenuLinks.push_back(make_tuple(k, *String(st.front()).Trim()));
					}
					else
					{
						Error = 24;
						return;
					}
					i++;
				}
			}
			catch (...)
			{
				Error = 25;
				return;
			}
		}


		t = ini.FindIniSection("DESKTOP_LINK");
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
						String k = st.front();
						k.Trim();
						st.pop_front();
						DesktopLinks.push_back(make_tuple(k, *String(st.front()).Trim()));
					}
					else
					{
						Error = 26;
						return;
					}
					i++;
				}
			}
			catch (...)
			{
				Error = 27;
				return;
			}
		}



		ini.RemoveAllSections();

	}
	else
		Error = 1;
}