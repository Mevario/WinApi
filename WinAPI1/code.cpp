#include <Windows.h>

TCHAR NazwaAplikacji[] = TEXT("Aplikacja studencka");
TCHAR NazwaKlasy[] = TEXT("OKNOGLOWNE");

//obs³uga komunikatów
static LRESULT CALLBACK FunkcjaOkienkowa(HWND Okno, UINT Komunikat, WPARAM wParam, LPARAM lParam);

//klasa g³ówna okna, 
static bool RejestrujKlasy()
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH) (1 + COLOR_WINDOW); //bia³e t³o
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //kursor strza³ka
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //standardowa ikona aplikacji
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = GetModuleHandle(NULL); // uchwyt instancji
	wc.lpfnWndProc = &FunkcjaOkienkowa; //funkcjia obs³ugi komunikatów
	wc.lpszClassName = NazwaKlasy;
	wc.lpszMenuName = NULL; //menu niepotrzebne na razie
	wc.style = 0;
	return (RegisterClassEx(&wc) != 0);
}

static void WyrejestrujKlasy()
{
	UnregisterClass(NazwaKlasy, GetModuleHandle(NULL));
}

int WINAPI WinMain(HINSTANCE Instancja, HINSTANCE Poprzednia, LPSTR Parametry, int Widocznosc)
{
	// Zarejestruj klasê. Protestuj, je¿eli wyst¹pi³ b³¹d.
	if (!RejestrujKlasy()) {
		MessageBox(NULL, TEXT("Nie uda³o siê zarejestrowaæ klasy okna!"),
			NazwaAplikacji, MB_ICONSTOP | MB_OK);
		return 1;
	}
	// Stwórz g³ówne okno. Równie¿ protestuj, je¿eli wyst¹pi³ b³¹d.
	HWND GlowneOkno = CreateWindowExW(WS_EX_APPWINDOW | WS_EX_CLIENTEDGE, NazwaKlasy, TEXT("Okno"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, Instancja, NULL);
	HWND PotomneOkno = CreateWindowExW(WS_EX_APPWINDOW | WS_EX_CLIENTEDGE, NazwaKlasy, TEXT("Okno"), WS_CHILD, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, Instancja, NULL);
	if (GlowneOkno == NULL) {
		MessageBox(NULL, TEXT("Nie uda³o siê stworzyæ g³ównego okna!"),
			NazwaAplikacji, MB_ICONSTOP | MB_OK);
		return 2;
	}
	// Wyœwietl i uaktualnij nowo stworzone okno.
	ShowWindow(GlowneOkno, Widocznosc);
	UpdateWindow(GlowneOkno);
	// G³ówna pêtla komunikatów w¹tku.
	MSG Komunikat;
	while (GetMessage(&Komunikat, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Komunikat);
		DispatchMessage(&Komunikat); 
	}
	// Zwolnij pamiêæ klas i zakoñcz proces.
	WyrejestrujKlasy();
	return static_cast<int>(Komunikat.wParam);
}

static LRESULT CALLBACK FunkcjaOkienkowa(HWND Okno, UINT Komunikat, WPARAM wParam, LPARAM lParam)
{
	HDC hdc; //uchwyt kontekstu
	PAINTSTRUCT ps;
	RECT rect;
	switch (Komunikat)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_PAINT:
			hdc = BeginPaint (Okno, &ps);
			GetClientRect(Okno, &rect);
			DrawTextW(hdc, TEXT ("A witam!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			EndPaint(Okno, &ps);
			break;
		case WM_LBUTTONDOWN:
			MessageBoxEx(Okno,TEXT("SIEMA"),TEXT("TEST"),NULL,NULL);
			break;
		default:
			return DefWindowProc(Okno, Komunikat, wParam, lParam);
	}
	return 0;
}