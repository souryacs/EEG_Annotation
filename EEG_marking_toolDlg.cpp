// EEG_marking_toolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EEG_marking_tool.h"
#include "EEG_marking_toolDlg.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//sourya - added define statements
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define MAX_LINE_LEN 2048	
#define NFFT 8192
#define TIME_INTERVAL 80	
#define CHAN_DATA_BLOCK_SIZE 1024

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEEG_marking_toolDlg dialog

CEEG_marking_toolDlg::CEEG_marking_toolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEEG_marking_toolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEEG_marking_toolDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEEG_marking_toolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEEG_marking_toolDlg)
	DDX_Control(pDX, IDC_CHN_4, m_chn4);
	DDX_Control(pDX, IDC_CHN_3, m_chn3);
	DDX_Control(pDX, IDC_CHN_2, m_chn2);
	DDX_Control(pDX, IDC_CHN_1, m_chn1);
	DDX_Control(pDX, IDC_STATIC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEEG_marking_toolDlg, CDialog)
	//{{AFX_MSG_MAP(CEEG_marking_toolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_VIEW_NEXT, OnViewNext)
	ON_COMMAND(ID_VIEW_PREV, OnViewPrev)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_TIMEBASE_6, OnTimebase6)
	ON_COMMAND(ID_SENSITIVITY_1, OnSensitivity1)
	ON_COMMAND(ID_SENSITIVITY_2, OnSensitivity2)
	ON_COMMAND(ID_SENSITIVITY_3, OnSensitivity3)
	ON_COMMAND(ID_SENSITIVITY_5, OnSensitivity5)
	ON_COMMAND(ID_SENSITIVITY_7, OnSensitivity7)
	ON_COMMAND(ID_SENSITIVITY_10, OnSensitivity10)
	ON_COMMAND(ID_SENSITIVITY_15, OnSensitivity15)
	ON_COMMAND(ID_SENSITIVITY_20, OnSensitivity20)
	ON_COMMAND(ID_SENSITIVITY_30, OnSensitivity30)
	ON_COMMAND(ID_SENSITIVITY_50, OnSensitivity50)
	ON_COMMAND(ID_SENSITIVITY_70, OnSensitivity70)
	ON_COMMAND(ID_SENSITIVITY_100, OnSensitivity100)
	ON_COMMAND(ID_SENSITIVITY_200, OnSensitivity200)
	ON_COMMAND(ID_SENSITIVITY_500, OnSensitivity500)
	ON_COMMAND(ID_TIMEBASE_8, OnTimebase8)
	ON_COMMAND(ID_TIMEBASE_10, OnTimebase10)
	ON_COMMAND(ID_TIMEBASE_15, OnTimebase15)
	ON_COMMAND(ID_TIMEBASE_20, OnTimebase20)
	ON_COMMAND(ID_TIMEBASE_30, OnTimebase30)
	ON_COMMAND(ID_TIMEBASE_60, OnTimebase60)
	ON_COMMAND(ID_TIMEBASE_120, OnTimebase120)
	ON_COMMAND(ID_TIMEBASE_240, OnTimebase240)
	ON_COMMAND(ID_MARKING_SEIZURE, OnMarkingSeizure)
	ON_COMMAND(ID_MARKING_BURST, OnMarkingBurst)
	ON_COMMAND(ID_MARKING_ARTIFACT, OnMarkingArtifact)
	ON_COMMAND(ID_MARKING_SLEEPSPINDLE, OnMarkingSleepspindle)
	ON_COMMAND(ID_MARKING_SLOWSHARPWAVE, OnMarkingSlowsharpwave)
	ON_COMMAND(ID_MARKING_UNKNOWNMARK, OnMarkingUnknownmark)
	ON_COMMAND(ID_MARKING_UNMARK, OnMarkingUnmark)
	ON_WM_KEYDOWN()
	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEEG_marking_toolDlg message handlers

BOOL CEEG_marking_toolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here	
	HDC screen = ::GetDC(NULL);
	int hSize = ::GetDeviceCaps(screen,HORZSIZE);
	int hRes = ::GetDeviceCaps(screen,HORZRES);
	PixelsPerMM = hRes/hSize;   // pixels per millimeter
	//double PixelsPerInch = PixelsPerMM * 25.4; //dpi

	// initialize global data pointers as NULL
	//channel_data = NULL;
	marking_database = NULL;

	pen.CreatePen(PS_SOLID, 5, RGB(255,0,0));

	// initialize color code for different marking
	// green
	color_code[0][0] = 0;
	color_code[0][1] = 255;
	color_code[0][2] = 0;
	// yellow
	color_code[1][0] = 255;
	color_code[1][1] = 255;
	color_code[1][2] = 0;
	// cyan
	color_code[2][0] = 0;
	color_code[2][1] = 128;
	color_code[2][2] = 255;
	// magenta
	color_code[3][0] = 128;
	color_code[3][1] = 0;
	color_code[3][2] = 0;
	// red
	color_code[4][0] = 255;
	color_code[4][1] = 0;
	color_code[4][2] = 0;
	// black
	color_code[5][0] = 0;
	color_code[5][1] = 0;
	color_code[5][2] = 0;

	for (int i = 0; i < 6; i++)
	{
		xpen[i].CreatePen(PS_SOLID, 3, RGB(color_code[i][0], color_code[i][1], color_code[i][2]));
	}

	m_Grid.GetClientRect(&rect);
	hWnd = m_Grid.m_hWnd;
	
	hDC = m_Grid.GetDC()->m_hDC;
	SelectObject(hDC, &pen);
	screen_y_size = rect.Height();
	screen_x_size = rect.Width();		
	
	drawBrush = CreateSolidBrush(RGB(255,255,255));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEEG_marking_toolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEEG_marking_toolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
        CPaintDC dc(this);
		plot_raw_EEG_signal();		//sourya
//		CDialog::OnPaint();
	}
}


// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEEG_marking_toolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CEEG_marking_toolDlg::OnFileOpen() 
{
	// TODO: Add your command handler code here

	// szFilters is a text string that includes two file name filters:
	static char BASED_CODE szFilters[]= "Text Files (*.txt)|*.txt|Ascii Files (*.ascii)|*.ascii||";

	//at first initialize several parameters related to future processing
	Init_Param_EEG();


	CFileDialog fOpenDlg(TRUE, "txt", "*.txt", OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);

	if(fOpenDlg.DoModal() == IDOK)
	{
		directory_name = fOpenDlg.GetPathName();
		fileName = fOpenDlg.GetFileName();
		directory_name.TrimRight(fileName);

		if (fileName.Find(".txt") != -1)
		{
			//text input data of RMS EEG machine
			SetWindowText("Raw RMS EEG data");
		}
		else if (fileName.Find(".ascii") != -1)
		{
			//amplitude EEG machine data
			SetWindowText("Raw AMP EEG data");
			
			//read amplitude EEG data
			Read_Text_Amp_EEG_machine_Config();
		}
	}
}


//************
//following functions are written by - sourya
void CEEG_marking_toolDlg::Init_Param_EEG(void)
{
	sensitivity = 10;   
	timebase = 10;		
	mark_on = 1;
	video_display = 0;
	mark_modified = 0;
	feature_extraction_marking_interval = 1;
	output_text_filename = "mark_manual.emdb";	//.emdb stands for eeg marked database
	total_recording_duration_sec = 0;
	filter_low_freq_pass = 1.6;
	filter_high_freq_pass = 35;	
	// list initializations
	list_nodecount = 0;
	head = NULL;
	tail = NULL;

} //end function

void CEEG_marking_toolDlg::Read_Text_Amp_EEG_machine_Config(void)
{
	char* token;
	char textLine[MAX_LINE_LEN];
	CString channel_info_filename;
	FILE* fid;
	CFileStatus status;

    // set the channel montage
    // read total no of channels
    channel_info_filename = directory_name + "montage.txt"; 
	if (CFile::GetStatus(channel_info_filename, status))
	{
		fid = fopen(channel_info_filename, "r");	//file open
        no_of_channels = 0;
        while (!feof (fid))
		{
			fgets(textLine, MAX_LINE_LEN, fid);            
            channel_name[no_of_channels] = textLine;
            token = strtok(textLine, "-\n");
            chan1_1st_channel[no_of_channels] = token;
            token = strtok(NULL, "-\n");     
            chan1_2nd_channel[no_of_channels] = token;
			no_of_channels++;
		} // end while
        fclose(fid);	//file close
	}
	else
	{
		return;
	}

    // call the channel detailed data read and processing function
    Read_and_Process_EEG_Data();    

} //end function


void CEEG_marking_toolDlg::Read_and_Process_EEG_Data(void)
{

	if (fileName.Find(".txt") != -1)
	{
		//Read_total_Input_RMS_raw();    //text file raw data read  
	}
	else if (fileName.Find(".ascii") != -1)
	{
        Read_Amp_EEG_machine_input_data();      //amplitude EEG file read

        Filter_Amp_EEG_Data();					// filter input data

		//AfxMessageBox("Amp EEG Data Read + Filtering Done");
	}

    // now plot the raw EEG signal
    plot_raw_EEG_signal();

} //end function


void CEEG_marking_toolDlg::Read_Amp_EEG_machine_input_data(void)
{
	FILE* fid;
	char textLine[MAX_LINE_LEN];
	char* token;
	int count, i;
	double temp_arr[MAX_NO_OF_CHANNELS];

	for (i = 0; i < MAX_NO_OF_CHANNELS; i++)
	{
		temp_arr[i] = 0;
	}

	// initialize sampling rate
	sampling_rate = 125;        

    // now we derive the individual channel index that occurs during the
    // channel name
    // for instance, in montage having channel C4-P4, we store index of
    // channel C4 in the structure chan1_index (because it is the 1st
    // channel) - otherwise, it is stored in the chan2_index
	// if montage contains REF string then corresponding channel index is set to 1
	for (i = 0; i < no_of_channels; i++)
	{
        if (!strcmp(chan1_1st_channel[i], "REF"))
		{
            chan1_index[i] = -1;
		}
        if (!strcmp(chan1_2nd_channel[i], "REF"))
		{
            chan2_index[i] = -1;
		}
	}

    fid = fopen(fileName, "r");     // file open
    fgets(textLine, MAX_LINE_LEN, fid);  //read the 1st line  - header information                 
    count = 0;               // string delimeter - channel name list
    token = strtok(textLine, " \t");

	//now set the channel index information
    for (; ((count < MAX_NO_OF_CHANNELS) && strcmp(token, "")); count++)   // at most MAX_NO_OF_CHANNELS no of different channels are selected
	{        
        for (i = 0; i < no_of_channels; i++)
		{
            if (!strcmp(chan1_1st_channel[i], token))
			{
                chan1_index[i] = count;
            }
            if (!strcmp(chan1_2nd_channel[i], token))
			{
                chan2_index[i] = count;
            }
        }
		token = strtok(NULL, " \t");
	}


	// find max channel index that is to be read 
	// for example, only one channel montage is recorded, that is C4-P4
	// so in the text file, reading upto max (C4_index, P4_index) is sufficient
	// this value is stored in the max channel index
	int max_channel_index = -1;
	for (i = 0; i < no_of_channels; i++)
	{
		if (chan1_index[i] > max_channel_index)
		{
			max_channel_index = chan1_index[i];
		}
		if (chan2_index[i] > max_channel_index)
		{
			max_channel_index = chan2_index[i];
		}
	}

	////////////////////////////////////

	// init the channel data size
	channel_data_size = 0;

	struct channnel_data_list *temp;	// temporary pointer to the new node 
	
	//at first allocate corresponding memory
	//channel_data = (double**) calloc(no_of_channels, sizeof(double*));

	while (!feof (fid))
	{
		fgets(textLine, MAX_LINE_LEN, fid);
		if (isalpha(textLine[0]))
			continue;

		if ((channel_data_size % CHAN_DATA_BLOCK_SIZE) == 0)
		{
			// allocate one node of list which will contain channel data 
			temp = (struct channnel_data_list *)malloc(sizeof(struct channnel_data_list));
			if (temp == NULL)
			{
				AfxMessageBox("Insufficient memory - no new node alloc");	
				return;
			}
			list_nodecount++;	// increment the node count

			temp->next = NULL;
			temp->prev = NULL;
			if (head == NULL && tail == NULL)	//  this is the first node
			{
				head = tail = temp;		// assign pointer
			}
			else
			{
				temp->prev = tail;
				tail->next = temp;
				tail = temp;			// assign pointer
			}
			
			// for each channel, allocate one chunk of memory
			tail->channel_data = (double**) calloc(no_of_channels, sizeof(double*));
			if (tail->channel_data == NULL)
			{
				AfxMessageBox("Insufficient memory - basic channel data alloc");	
				return;
			}
			for (i = 0; i < no_of_channels; i++)
			{
				tail->channel_data[i] = (double*) calloc(CHAN_DATA_BLOCK_SIZE, sizeof(double));
				if (tail->channel_data[i] == NULL)
				{
					AfxMessageBox("Insufficient memory - no new channel data alloc");	
					return;
				}
			}
		}

		token = strtok(textLine, " \t");	
		i = 0;

		//read one line data and store it in temporary array
		while (!( (token == NULL) || (i >= max_channel_index /* MAX_NO_OF_CHANNELS */) ))
		{
			temp_arr[i] = ((atof(token) + 4999.84)/0.152587771233253)-32767;
			token = strtok(NULL, " \t");
			i++;
		}
		// insert 0's for any non-available data
		for (; i < max_channel_index /* MAX_NO_OF_CHANNELS */; i++)
		{
			temp_arr[i] = 0;
		}

		// now insert the line data onto channel data structure based on the channel no index obtained 
		for (i = 0; i < no_of_channels; i++)
		{
			if ((chan1_index[i] == -1) && (chan2_index[i] == -1))		//REF-REF
			{
				tail->channel_data[i][channel_data_size % CHAN_DATA_BLOCK_SIZE] = 0;
			}
			else if ((chan1_index[i] != -1) && (chan2_index[i] != -1))	//NON_REF-NON_REF
			{
				tail->channel_data[i][channel_data_size % CHAN_DATA_BLOCK_SIZE] = (temp_arr[chan1_index[i]] - temp_arr[chan2_index[i]]);
			}
			else if ((chan1_index[i] == -1) && (chan2_index[i] != -1))	//REF-NON_REF
			{
				tail->channel_data[i][channel_data_size % CHAN_DATA_BLOCK_SIZE] = -temp_arr[chan2_index[i]];
			}
			else if ((chan1_index[i] != -1) && (chan2_index[i] == -1))	//NON_REF-REF
			{
				tail->channel_data[i][channel_data_size % CHAN_DATA_BLOCK_SIZE] = temp_arr[chan1_index[i]];
			}
		}	//end one line read and assign

		// increment the counter
		channel_data_size++;

	}	// end file read loop

	// file close
	fclose(fid); 
    
	//////////////////////

	time_interval = (1 / sampling_rate);
	total_recording_duration_sec = (channel_data_size / sampling_rate);  

    // raw EEG displays according to screen size and time base
    raw_eeg_per_page_duration = (int)(screen_x_size / (PixelsPerMM * timebase));   // (in Sec) time
    if ( raw_eeg_per_page_duration > total_recording_duration_sec )
	{
        raw_eeg_per_page_duration = (int)total_recording_duration_sec;
	}   
    sel_raw_eeg_start_time = 0;
    sel_raw_eeg_end_time = raw_eeg_per_page_duration;

	/////////////////////////////
	//initialize the marking database count
	for (i = 0; i < MAX_NO_OF_CHANNELS; i++)
	{
		mark_database_count[i] = 0;
	}
	//initialize the marking database 
	marking_database = (double***) malloc(no_of_channels * sizeof(double**));
	for (i = 0; i < no_of_channels; i++)
	{
		marking_database[i] = (double**) malloc((int)total_recording_duration_sec * sizeof(double*));
		for (count = 0; count < total_recording_duration_sec; count++)
		{
			marking_database[i][count] = (double*) malloc(3 * sizeof(double));
		}
	}
	/////////////////////////////
	// now open the output excel file if it already exists
	CString outfile = directory_name + output_text_filename; 	
	CFileStatus status;
	double start_time, end_time, mark;
	int ch_no;

	if (CFile::GetStatus(outfile, status))	//if file exists
	{
		fid = fopen(outfile, "r");	//file open	in read mode
		while (!feof (fid))	
		{
			fscanf(fid, "%lf %lf %lf %d", &start_time, &end_time, &mark, &ch_no);
			marking_database[ch_no][mark_database_count[ch_no]][0] = start_time;
			marking_database[ch_no][mark_database_count[ch_no]][1] = end_time;
			marking_database[ch_no][mark_database_count[ch_no]][2] = mark;
			mark_database_count[ch_no] = mark_database_count[ch_no] + 1;
		}
		fclose(fid);
	}
	/////////////////////////////

} //end function


// filtering function
void CEEG_marking_toolDlg::Filter_Amp_EEG_Data()
{
	// calculate kaiser window parameters
	int kaiserWindowLength;
	double beta;
	double ripple = 0.001;	//0.0001;			// ripple
	double transWidth = 0.5;	//0.1;			// pass band step
	double dw = 2 * M_PI * transWidth / sampling_rate;	// Calculate delta w
	double a = -20.0 * log10(ripple);	// Calculate ripple dB
	int m;				// Calculated filter order
	
	if (a > 21) 
	{
		m = (int)ceil((a - 7.95) / (2.285 * dw));
	}
	else 
	{
		m = (int)ceil(5.79 / dw);
	}
	kaiserWindowLength = m + 1;
	if (a <= 21) 
	{
		beta = 0.0;
	}
	else if (a <= 50)
	{
		beta = 0.5842 * pow(a - 21, 0.4) + 0.07886 * (a - 21);
	}
	else 
	{
		beta = 0.1102 * (a - 8.7);
	}

	// calculate sinc window
	double *bpf = (double *) malloc(kaiserWindowLength * sizeof(double));
	double ft1 = filter_low_freq_pass / sampling_rate;
	double ft2 = filter_high_freq_pass / sampling_rate;
	double m_2 = 0.5 * (kaiserWindowLength - 1);
	int halfLength = kaiserWindowLength / 2;
	int n;
	double val, val1, val2, denom;


	// Set centre tap, if present
	// This avoids a divide by zero
	if (2 * halfLength != kaiserWindowLength) 
	{
		val = 2.0 * (ft2 - ft1);
		bpf[halfLength] = val;
	}

	// Calculate taps
	// Due to symmetry, only need to calculate half the window
	for (n = 0; n < halfLength; n++) 
	{
		val1 = sin(2.0 * M_PI * ft1 * (n - m_2)) / (M_PI * (n - m_2));
		val2 = sin(2.0 * M_PI * ft2 * (n - m_2)) / (M_PI * (n - m_2));

		bpf[n] = val2 - val1;
		bpf[kaiserWindowLength - n - 1] = val2 - val1;
	}


	double *bpf_kaiser = (double *)malloc(kaiserWindowLength * sizeof(double));
	m_2 = 0.5 * (kaiserWindowLength - 1);
	denom = modZeroBessel(beta);					// Denominator of Kaiser function

	for (n = 0; n < kaiserWindowLength; n++)
	{
		val = ((n) - m_2) / m_2;
		val = 1 - (val * val);
		bpf_kaiser[n] = modZeroBessel(beta * sqrt(val)) / denom;
	}

	for (n = 0; n < kaiserWindowLength; n++) 
	{ 
		bpf_kaiser[n] *= bpf[n];
	}
	
	/// now apply the kaiser window onto the input signal
	int nx = (int)sampling_rate * TIME_INTERVAL;		// 16 sec data
	int ny = nx + kaiserWindowLength - 1;
	int filt_offset = (kaiserWindowLength / 2);

	// now initialize output storage for filter
	//double* y = (double *) malloc(nx * sizeof(double));	//output filter 
	double* y = (double *) malloc(ny * sizeof(double));	//output filter 
	double* x = (double *) malloc(nx * sizeof(double));	//input data store

	double* y_real = (double *) malloc(NFFT * sizeof(double));	//output filter 
	double* x_real = (double *) malloc(NFFT * sizeof(double));	//input data store
	double* y_im = (double *) malloc(NFFT * sizeof(double));	//output filter 
	double* x_im = (double *) malloc(NFFT * sizeof(double));	//input data store

	int ch_no, time_elapsed;
	long start_index, end_index;
	int i, j;

	// calculate the fourier transform of the filter
	double* filtered_data_fft_real = (double *) malloc(NFFT * sizeof(double));	//filter real part
	double* filtered_data_fft_im = (double *) malloc(NFFT * sizeof(double));	//filter imaginary part	
	for (i = 0; i < kaiserWindowLength; i++)
	{
		filtered_data_fft_real[i] = bpf_kaiser[i];
		filtered_data_fft_im[i] = 0;
	}
	for (i = kaiserWindowLength; i < NFFT; i++)
	{
		filtered_data_fft_real[i] = 0;
		filtered_data_fft_im[i] = 0;
	}
	// perform the FFT of filter
	FFT_C(1, filtered_data_fft_real, filtered_data_fft_im);	


	struct channnel_data_list *read_temp;	// temporary pointer to the new node 
	struct channnel_data_list *write_temp;	// temporary pointer to the new node 
	int start_node_num, end_node_num;
	int node_count;
	int start_node_offset, end_node_offset;

	// calculate filtered data
	for (ch_no = 0; ch_no < no_of_channels; ch_no++)
	{
		// for each channel, point the temporary pointer to the head of the channel data
		read_temp = head;
		write_temp = head;

		// we traverse input data by 1 sec intervals
		for (time_elapsed = 0; time_elapsed < total_recording_duration_sec; (time_elapsed = time_elapsed + TIME_INTERVAL))
		{
			start_index = (int)(time_elapsed * sampling_rate);
			end_index = (int)((time_elapsed + TIME_INTERVAL) * sampling_rate - 1);
			if (end_index >= channel_data_size)
			{
				end_index = channel_data_size - 1;
			}
			nx = end_index - start_index + 1;
			ny = nx + kaiserWindowLength - 1;

			// start offset of the current node from which data will be read
			start_node_offset = start_index % CHAN_DATA_BLOCK_SIZE;
			end_node_offset = end_index % CHAN_DATA_BLOCK_SIZE;
			start_node_num = start_index / CHAN_DATA_BLOCK_SIZE;
			end_node_num = end_index / CHAN_DATA_BLOCK_SIZE;

			// for storing input data
			i = 0;
			for (node_count = start_node_num; node_count <= end_node_num; node_count++) 
			{
				if (node_count == end_node_num)		// this is the last node - may or may not be completely processed
				{
					for (j = start_node_offset; j <= end_node_offset; j++, i++)	//start node offset is 0 or set value
						x[i] = read_temp->channel_data[ch_no][j];

					if (end_node_offset == (CHAN_DATA_BLOCK_SIZE - 1))	/// this node is completely processed
						read_temp = read_temp->next;					
				}
				else if (node_count == start_node_num)		// starting node but not end node
				{
					for (j = start_node_offset; j < CHAN_DATA_BLOCK_SIZE; j++, i++)
						x[i] = read_temp->channel_data[ch_no][j];

					read_temp = read_temp->next;
				}
				else
				{
					for (j = start_node_offset; j < CHAN_DATA_BLOCK_SIZE; j++, i++)
						x[i] = read_temp->channel_data[ch_no][j];

					read_temp = read_temp->next;
				}
				start_node_offset = 0;
			}

			// reset output data
			for (i = 0; i < ny; i++)
				y[i] = 0;

			//////////////////////////
			// fftfilt function implemented in C
			fftfilt_C_impl(filtered_data_fft_real, filtered_data_fft_im, x, y, nx, ny, kaiserWindowLength, x_real, x_im, y_real, y_im);		
			//////////////////////////

			// now filtered data will be placed back to the original input signal
			start_node_offset = start_index % CHAN_DATA_BLOCK_SIZE;

			i = 0;
			for (node_count = start_node_num; node_count <= end_node_num; node_count++) 
			{
				if (node_count == end_node_num)		// this is the last node - may or may not be completely processed
				{
					for (j = start_node_offset; j <= end_node_offset; j++, i++)	//start node offset is 0 or set value
						write_temp->channel_data[ch_no][j] = y[i + filt_offset] * (-1);	//(-0.1);

					if (end_node_offset == (CHAN_DATA_BLOCK_SIZE - 1))	/// this node is completely processed
						write_temp = write_temp->next;					
				}
				else if (node_count == start_node_num)		// starting node but not end node
				{
					for (j = start_node_offset; j < CHAN_DATA_BLOCK_SIZE; j++, i++)
						write_temp->channel_data[ch_no][j] = y[i + filt_offset] * (-1);	//(-0.1);

					write_temp = write_temp->next;
				}
				else
				{
					for (j = start_node_offset; j < CHAN_DATA_BLOCK_SIZE; j++, i++)
						write_temp->channel_data[ch_no][j] = y[i + filt_offset] * (-1);	//(-0.1);

					write_temp = write_temp->next;
				}
				start_node_offset = 0;
			}
			// end of filtered data store

		}	// end time elapsed loop	
	} // end all channel filtering loop

	// free the memory
	free(y_im);
	free(y_real);
	free(x_im);
	free(x_real);
	free(y);
	free(x);

} //end function

double CEEG_marking_toolDlg::modZeroBessel(double x)
{
	int i;

	double x_2 = x/2;
	double num = 1;
	double fact = 1;
	double result = 1;

	for (i=1 ; i<20 ; i++) {
		num *= x_2 * x_2;
		fact *= i;
		result += num / (fact * fact);
	}
	return result;
}

/*
	fftfilt function in MATLAB is implemented in C
*/	

void CEEG_marking_toolDlg::fftfilt_C_impl(double* filt_real, double* filt_im, double* inp_sig, double* out_sig, int in_sig_len, int out_sig_len, int filt_sig_len, double* x_real, double* x_im, double* y_real, double* y_im)
{
	int L = NFFT - filt_sig_len + 1;	// fft level
	int istart, iend, yend;
	int i;

	istart = 0;
	while (istart < in_sig_len)
	{		
		iend = ((istart + L - 1) >= in_sig_len) ? (in_sig_len - 1) : (istart + L - 1);	// min operator
		if (iend == istart)
		{
			// fill the x_real and x_im portions - NFFT no of 1's
			for (i = 0; i < NFFT; i++)
			{
				x_real[i] = 1; 
				x_im[i] = 0;
			}
		}
		else
		{
			for (i = istart; i <= iend; i++)
			{
				x_real[i - istart] = inp_sig[i]; 
				x_im[i - istart] = 0;
			}
			for (i = iend + 1; i <= (istart + NFFT - 1); i++)
			{
				x_real[i - istart] = 0; 
				x_im[i - istart] = 0;
			}
			FFT_C(1, x_real, x_im);
		}
		// multiplication of two real and imaginary component arrays and storing it into 3rd array
		for (i = 0; i < NFFT; i++)
		{
			y_real[i] = x_real[i] * filt_real[i] - x_im[i] * filt_im[i];
			y_im[i] = x_real[i] * filt_im[i] + x_im[i] * filt_real[i];
		}

		// inverse FFT operation on output array
		FFT_C(-1, y_real, y_im);

		// copy the real part of the inverse FFT output to the output Y array
		//yend = ((istart + NFFT - 1) >= in_sig_len) ? (in_sig_len - 1) : (istart + NFFT - 1);	// min operator
		yend = ((istart + NFFT - 1) >= (in_sig_len + filt_sig_len - 1)) ? (in_sig_len + filt_sig_len - 2) : (istart + NFFT - 1);	// min operator
		for (i = istart; i <= yend; i++)
		{
			out_sig[i] = out_sig[i] + y_real[i - istart];	
		}
		istart = istart + L;
	}
}	// end of fftfilt function


/* 
	FFT implementation in C code 
*/

void CEEG_marking_toolDlg::FFT_C(int dir, double *x, double *y)
{
   long n,i,i1,j,k,i2,l,l1,l2;
   double c1,c2,tx,ty,t1,t2,u1,u2,z;
   double m = log(NFFT) / log(2);

   /* Calculate the number of points */
   n = NFFT;

   /* Do the bit reversal */
   i2 = n >> 1;
   j = 0;
   for (i=0;i<n-1;i++) {
      if (i < j) {
         tx = x[i];
         ty = y[i];
         x[i] = x[j];
         y[i] = y[j];
         x[j] = tx;
         y[j] = ty;
      }
      k = i2;
      while (k <= j) {
         j -= k;
         k >>= 1;
      }
      j += k;
   }

   /* Compute the FFT */
   c1 = -1.0; 
   c2 = 0.0;
   l2 = 1;
   for (l=0;l<m;l++) {
      l1 = l2;
      l2 <<= 1;
      u1 = 1.0; 
      u2 = 0.0;
      for (j=0;j<l1;j++) {
         for (i=j;i<n;i+=l2) {
            i1 = i + l1;
            t1 = u1 * x[i1] - u2 * y[i1];
            t2 = u1 * y[i1] + u2 * x[i1];
            x[i1] = x[i] - t1; 
            y[i1] = y[i] - t2;
            x[i] += t1;
            y[i] += t2;
         }
         z =  u1 * c1 - u2 * c2;
         u2 = u1 * c2 + u2 * c1;
         u1 = z;
      }
      c2 = sqrt((1.0 - c1) / 2.0);
      if (dir == 1) 
         c2 = -c2;
      c1 = sqrt((1.0 + c1) / 2.0);
   }

   /* Scaling for forward transform */
   if (dir == -1) {
      for (i=0;i<n;i++) {
         x[i] /= n;
         y[i] /= n;
      }
   } 
}


/*
	plotting raw EEG signal data
*/
void CEEG_marking_toolDlg::plot_raw_EEG_signal(void)
{
	CPen *oldPen;
	CDC *pDC = m_Grid.GetDC();
	int startx, endx, starty, endy;

	SelectObject(hDC, &pen);
    FillRect(hDC, &rect, drawBrush);

    // plot x and y
    int x_start_index = 0;
    int x_end_index = (int)((sel_raw_eeg_end_time - sel_raw_eeg_start_time) * sampling_rate) - 1;
    int y_start_index = (int)(sel_raw_eeg_start_time * sampling_rate);
    int y_end_index = y_start_index + (x_end_index - x_start_index);

	int node_count;
	int start_node_offset, end_node_offset, start_node_num, end_node_num;

	struct channnel_data_list *temp1;
	struct channnel_data_list *temp2;

	int i, j, n;
	double x_axis_res_graph = (x_end_index - x_start_index + 1) / screen_x_size;
	double y_axis_res_graph = sensitivity / PixelsPerMM;			
	double origin_pos_per_channel;

	start_node_offset = y_start_index % CHAN_DATA_BLOCK_SIZE;
	end_node_offset = y_end_index % CHAN_DATA_BLOCK_SIZE;
	start_node_num = y_start_index / CHAN_DATA_BLOCK_SIZE;
	end_node_num = y_end_index / CHAN_DATA_BLOCK_SIZE;

	temp1 = head;	//initialization
	// temp1 should reach to the starting node
	for (i = 0; i < start_node_num; i++)
	{
		temp1 = temp1->next;
	}

	//////////////////////////////
	// plot the grid lines	
	for (n = 0; n <= raw_eeg_per_page_duration; n++)
	{
		//move to x1, y1
		::MoveToEx(hDC, (int)((n * screen_x_size) / raw_eeg_per_page_duration), 0, NULL);		

		//line to x2, y2
		LineTo(hDC, (int)((n * screen_x_size) / raw_eeg_per_page_duration), screen_y_size);
	}
	//////////////////////////////
	// plot the graph
	for (n = 0; n < no_of_channels; n++)
	{
		start_node_offset = y_start_index % CHAN_DATA_BLOCK_SIZE;

		// set channel origin for graph display
		origin_pos_per_channel = ((n + 0.5) * screen_y_size / no_of_channels);	

		// initialize the node pointer with the staring node
		temp2 = temp1;	//plot individual data

		i = 0;
		for (node_count = start_node_num; node_count <= end_node_num; node_count++) 
		{
			if (node_count == end_node_num)		// this is the last node - may or may not be completely processed
			{
				for (j = start_node_offset; j <= (end_node_offset - 1); j++, i++)	//start node offset is 0 or set value			
				{
					::MoveToEx(hDC, (int)((i - x_start_index) / x_axis_res_graph), (int)((temp2->channel_data[n][j] / y_axis_res_graph) + origin_pos_per_channel), NULL);		
					LineTo(hDC, (int)((i + 1 - x_start_index) / x_axis_res_graph), (int)((temp2->channel_data[n][j+1] / y_axis_res_graph) + origin_pos_per_channel));
				}
				if (end_node_offset == (CHAN_DATA_BLOCK_SIZE - 1))	/// this node is completely processed
					temp2 = temp2->next;					
			}
			else if (node_count == start_node_num)		// starting node but not end node
			{
				for (j = start_node_offset; j < (CHAN_DATA_BLOCK_SIZE - 1); j++, i++)
				{
					::MoveToEx(hDC, (int)((i - x_start_index) / x_axis_res_graph), (int)((temp2->channel_data[n][j] / y_axis_res_graph) + origin_pos_per_channel), NULL);		
					LineTo(hDC, (int)((i + 1 - x_start_index) / x_axis_res_graph), (int)((temp2->channel_data[n][j+1] / y_axis_res_graph) + origin_pos_per_channel));
				}					
				::MoveToEx(hDC, (int)((i - x_start_index) / x_axis_res_graph), (int)((temp2->channel_data[n][j] / y_axis_res_graph) + origin_pos_per_channel), NULL);		
				temp2 = temp2->next;
				LineTo(hDC, (int)((i + 1 - x_start_index) / x_axis_res_graph), (int)((temp2->channel_data[n][0] / y_axis_res_graph) + origin_pos_per_channel));
				i++;
			}
			else
			{
				for (j = start_node_offset; j < (CHAN_DATA_BLOCK_SIZE - 1); j++, i++)
				{
					::MoveToEx(hDC, (int)((i - x_start_index) / x_axis_res_graph), (int)((temp2->channel_data[n][j] / y_axis_res_graph) + origin_pos_per_channel), NULL);		
					LineTo(hDC, (int)((i + 1 - x_start_index) / x_axis_res_graph), (int)((temp2->channel_data[n][j+1] / y_axis_res_graph) + origin_pos_per_channel));
				}					
				::MoveToEx(hDC, (int)((i - x_start_index) / x_axis_res_graph), (int)((temp2->channel_data[n][j] / y_axis_res_graph) + origin_pos_per_channel), NULL);		
				temp2 = temp2->next;
				LineTo(hDC, (int)((i + 1 - x_start_index) / x_axis_res_graph), (int)((temp2->channel_data[n][0] / y_axis_res_graph) + origin_pos_per_channel));
				i++;
			}
			start_node_offset = 0;
		}
		//m_chn1.SetWindowText(channel_name[0]);
		//m_chn1.SetWindowPos(NULL, (screen_x_size + 1), (int)origin_pos_per_channel, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
	}

	//////////////////////////////
	// mark the marked portion
	for (n = 0; n < no_of_channels; n++)
	{
		for (i = 0; i < mark_database_count[n]; i++)
		{
			if (/* (marking_database[n][i][0] != -1) && */ (marking_database[n][i][0] >= sel_raw_eeg_start_time) && (marking_database[n][i][1] <= sel_raw_eeg_end_time))
			{
				startx = (int)((marking_database[n][i][0] - sel_raw_eeg_start_time ) * screen_x_size) / raw_eeg_per_page_duration;
				endx = (int)((marking_database[n][i][1] - sel_raw_eeg_start_time ) * screen_x_size) / raw_eeg_per_page_duration;
				starty = (n * screen_y_size) / no_of_channels;
				endy = ((n + 1) * screen_y_size) / no_of_channels;

				oldPen = pDC->SelectObject(&xpen[(int)marking_database[n][i][2]]);

				pDC->MoveTo(startx, starty);		
				pDC->LineTo(endx, starty);
				pDC->MoveTo(endx, starty);		
				pDC->LineTo(endx, endy);
				pDC->MoveTo(endx, endy);		
				pDC->LineTo(startx, endy);
				pDC->MoveTo(startx, endy);		
				pDC->LineTo(startx, starty);

				pDC->SelectObject(oldPen);
			}
		}
	}
	//////////////////////////////

} //end function
//************

/*
	next page raw EEG display
*/
void CEEG_marking_toolDlg::OnViewNext(int flag) 
{
	// TODO: Add your command handler code here
	// flag variable - if 0, next page is displayed
	// if 1 then only 1 second displacement is done

	if (sel_raw_eeg_end_time != total_recording_duration_sec)	//end of file is not reached yet
	{
		//update the current raw EEG display boundary
		if (flag == 0)
			sel_raw_eeg_start_time = sel_raw_eeg_end_time;
		else
			sel_raw_eeg_start_time = sel_raw_eeg_start_time + 1;

		sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
		if (sel_raw_eeg_end_time > total_recording_duration_sec)	//crosses time
		{
			sel_raw_eeg_end_time = total_recording_duration_sec;
			sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
		}
		//call plot raw EEG
		plot_raw_EEG_signal();
	}
}

/*
	prev page raw EEG display
*/
void CEEG_marking_toolDlg::OnViewPrev(int flag) 
{
	// TODO: Add your command handler code here
	// flag variable - if 0, prev page is displayed
	// if 1 then only 1 second displacement is done

	if (sel_raw_eeg_start_time != 0)	//start of file is not reached yet
	{
		//update the current raw EEG display boundary
		if (flag == 0)
			sel_raw_eeg_end_time = sel_raw_eeg_start_time;
		else
			sel_raw_eeg_end_time = sel_raw_eeg_end_time -  1;

		sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
		if (sel_raw_eeg_start_time < 0)
		{
			sel_raw_eeg_start_time = 0;
			sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
		}
		//call plot raw EEG
		plot_raw_EEG_signal();
	}	
}

/*
	mouse click handler event
*/
void CEEG_marking_toolDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CPen *oldPen;
	CDC *pDC = m_Grid.GetDC();
	int xpt, ypt, startx, endx, starty, endy;
	int marked_eeg_channel_no;
	int marked_eeg_interval_start_time, marked_eeg_interval_end_time;

	// TODO: Add your message handler code here and/or call default
	xpt = point.x;
	ypt = point.y;
	marked_eeg_channel_no = (ypt / (screen_y_size / no_of_channels));	
	marked_eeg_interval_start_time = (int)(((xpt * raw_eeg_per_page_duration) / screen_x_size) + sel_raw_eeg_start_time);      
	marked_eeg_interval_end_time = marked_eeg_interval_start_time + feature_extraction_marking_interval;
	startx = (int)((marked_eeg_interval_start_time - sel_raw_eeg_start_time ) * screen_x_size) / raw_eeg_per_page_duration;
	endx = (int)((marked_eeg_interval_end_time - sel_raw_eeg_start_time ) * screen_x_size) / raw_eeg_per_page_duration;
	starty = (marked_eeg_channel_no * screen_y_size) / no_of_channels;
	endy = ((marked_eeg_channel_no + 1) * screen_y_size) / no_of_channels;
	
	if (mark_on != 6)		// mark is other than unmark
	{
		oldPen = pDC->SelectObject(&xpen[mark_on]);

		// rectangle
		pDC->MoveTo(startx, starty);		
		pDC->LineTo(endx, starty);
		pDC->MoveTo(endx, starty);		
		pDC->LineTo(endx, endy);
		pDC->MoveTo(endx, endy);		
		pDC->LineTo(startx, endy);
		pDC->MoveTo(startx, endy);		
		pDC->LineTo(startx, starty);

		pDC->SelectObject(oldPen);
	}
	else	// unmark data
	{
		oldPen = pDC->SelectObject(&xpen[mark_on - 1]);

		// cross mark
		pDC->MoveTo(startx, starty);
		pDC->LineTo(endx, endy);
		pDC->MoveTo(endx, starty);
		pDC->LineTo(startx, endy);
		
		pDC->SelectObject(oldPen);
	}
	/////////////////
	// append in the marking database if existing data is not modified
	// otherwise update the database
	bool existing_data_modifed = 0;

	for (int i = 0; i < mark_database_count[marked_eeg_channel_no]; i++)
	{
		if ((marked_eeg_interval_start_time == marking_database[marked_eeg_channel_no][i][0]) && (marked_eeg_interval_end_time == marking_database[marked_eeg_channel_no][i][1]))
		{
			existing_data_modifed = 1;	
			if (mark_on == 6)	//unmark existing mark
			{
				marking_database[marked_eeg_channel_no][i][0] = -1;
				marking_database[marked_eeg_channel_no][i][1] = -1;
				marking_database[marked_eeg_channel_no][i][2] = -1;
			}
			else
			{
				marking_database[marked_eeg_channel_no][i][2] = mark_on;
			}
			break;
		}
	}

	if (existing_data_modifed == 0)
	{
		// first fill the current EEG timing and mark
		marking_database[marked_eeg_channel_no][mark_database_count[marked_eeg_channel_no]][0] = marked_eeg_interval_start_time;
		marking_database[marked_eeg_channel_no][mark_database_count[marked_eeg_channel_no]][1] = marked_eeg_interval_end_time;
		marking_database[marked_eeg_channel_no][mark_database_count[marked_eeg_channel_no]][2] = mark_on;

		// update the counter
		mark_database_count[marked_eeg_channel_no] = mark_database_count[marked_eeg_channel_no] + 1;
	}

	/////////////////
	CDialog::OnLButtonDown(nFlags, point);
}

/*
	this function is called when we call marking data save 
*/
void CEEG_marking_toolDlg::OnFileSave() 
{
	// TODO: Add your command handler code here
	FILE* fid;
	CString outfile = directory_name + output_text_filename; 

	//we save the marking database in one excel file
	int channel_no, i;

	fid = fopen(outfile, "w");	//file open	

	for (channel_no = 0; channel_no < no_of_channels; channel_no++)
	{
		for (i = 0; i < mark_database_count[channel_no]; i++)
		{
			if (marking_database[channel_no][i][0] != -1)	// valid entry
				fprintf(fid, "%f\t%f\t%f\t%d\n", marking_database[channel_no][i][0], marking_database[channel_no][i][1], marking_database[channel_no][i][2], channel_no);
		}
	}
	fclose(fid);
	AfxMessageBox("File Saved !");
}

void CEEG_marking_toolDlg::OnTimebase6() 
{
	// TODO: Add your command handler code here
	timebase = 6;		
    // raw EEG displays according to screen size and time base
    raw_eeg_per_page_duration = (int)(screen_x_size / (PixelsPerMM * timebase));   // (in Sec) time
    if ( raw_eeg_per_page_duration > total_recording_duration_sec )
	{
        raw_eeg_per_page_duration = (int)total_recording_duration_sec;
	}   
    sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
	if (sel_raw_eeg_end_time > total_recording_duration_sec)
	{
		sel_raw_eeg_end_time = total_recording_duration_sec;
		sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
	}
	plot_raw_EEG_signal();
}



void CEEG_marking_toolDlg::OnSensitivity1() 
{
	// TODO: Add your command handler code here
	sensitivity = 1;   
	plot_raw_EEG_signal();
}

void CEEG_marking_toolDlg::OnSensitivity2() 
{
	// TODO: Add your command handler code here
	sensitivity = 2;   
	plot_raw_EEG_signal();	
}

void CEEG_marking_toolDlg::OnSensitivity3() 
{
	// TODO: Add your command handler code here
	sensitivity = 3;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity5() 
{
	// TODO: Add your command handler code here
	sensitivity = 5;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity7() 
{
	// TODO: Add your command handler code here
	sensitivity = 7;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity10() 
{
	// TODO: Add your command handler code here
	sensitivity = 10;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity15() 
{
	// TODO: Add your command handler code here
	sensitivity = 15;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity20() 
{
	// TODO: Add your command handler code here
	sensitivity = 20;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity30() 
{
	// TODO: Add your command handler code here
	sensitivity = 30;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity50() 
{
	// TODO: Add your command handler code here
	sensitivity = 50;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity70() 
{
	// TODO: Add your command handler code here
	sensitivity = 70;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity100() 
{
	// TODO: Add your command handler code here
	sensitivity = 100;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity200() 
{
	// TODO: Add your command handler code here
	sensitivity = 200;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnSensitivity500() 
{
	// TODO: Add your command handler code here
	sensitivity = 500;   
	plot_raw_EEG_signal();		
}

void CEEG_marking_toolDlg::OnTimebase8() 
{
	// TODO: Add your command handler code here
	timebase = 8;		
    // raw EEG displays according to screen size and time base
    raw_eeg_per_page_duration = (int)(screen_x_size / (PixelsPerMM * timebase));   // (in Sec) time
    if ( raw_eeg_per_page_duration > total_recording_duration_sec )
	{
        raw_eeg_per_page_duration = (int)total_recording_duration_sec;
	}   
    sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
	if (sel_raw_eeg_end_time > total_recording_duration_sec)
	{
		sel_raw_eeg_end_time = total_recording_duration_sec;
		sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
	}
	plot_raw_EEG_signal();	
}

void CEEG_marking_toolDlg::OnTimebase10() 
{
	// TODO: Add your command handler code here
	timebase = 10;		
    // raw EEG displays according to screen size and time base
    raw_eeg_per_page_duration = (int)(screen_x_size / (PixelsPerMM * timebase));   // (in Sec) time
    if ( raw_eeg_per_page_duration > total_recording_duration_sec )
	{
        raw_eeg_per_page_duration = (int)total_recording_duration_sec;
	}   
    sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
	if (sel_raw_eeg_end_time > total_recording_duration_sec)
	{
		sel_raw_eeg_end_time = total_recording_duration_sec;
		sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
	}
	plot_raw_EEG_signal();	
}

void CEEG_marking_toolDlg::OnTimebase15() 
{
	// TODO: Add your command handler code here
	timebase = 15;		
    // raw EEG displays according to screen size and time base
    raw_eeg_per_page_duration = (int)(screen_x_size / (PixelsPerMM * timebase));   // (in Sec) time
    if ( raw_eeg_per_page_duration > total_recording_duration_sec )
	{
        raw_eeg_per_page_duration = (int)total_recording_duration_sec;
	}   
    sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
	if (sel_raw_eeg_end_time > total_recording_duration_sec)
	{
		sel_raw_eeg_end_time = total_recording_duration_sec;
		sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
	}
	plot_raw_EEG_signal();	
}

void CEEG_marking_toolDlg::OnTimebase20() 
{
	// TODO: Add your command handler code here
	timebase = 20;		
    // raw EEG displays according to screen size and time base
    raw_eeg_per_page_duration = (int)(screen_x_size / (PixelsPerMM * timebase));   // (in Sec) time
    if ( raw_eeg_per_page_duration > total_recording_duration_sec )
	{
        raw_eeg_per_page_duration = (int)total_recording_duration_sec;
	}   
    sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
	if (sel_raw_eeg_end_time > total_recording_duration_sec)
	{
		sel_raw_eeg_end_time = total_recording_duration_sec;
		sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
	}
	plot_raw_EEG_signal();	
}

void CEEG_marking_toolDlg::OnTimebase30() 
{
	// TODO: Add your command handler code here
	timebase = 30;		
    // raw EEG displays according to screen size and time base
    raw_eeg_per_page_duration = (int)(screen_x_size / (PixelsPerMM * timebase));   // (in Sec) time
    if ( raw_eeg_per_page_duration > total_recording_duration_sec )
	{
        raw_eeg_per_page_duration = (int)total_recording_duration_sec;
	}   
    sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
	if (sel_raw_eeg_end_time > total_recording_duration_sec)
	{
		sel_raw_eeg_end_time = total_recording_duration_sec;
		sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
	}
	plot_raw_EEG_signal();	
}

void CEEG_marking_toolDlg::OnTimebase60() 
{
	// TODO: Add your command handler code here
	timebase = 60;		
    // raw EEG displays according to screen size and time base
    raw_eeg_per_page_duration = (int)(screen_x_size / (PixelsPerMM * timebase));   // (in Sec) time
    if ( raw_eeg_per_page_duration > total_recording_duration_sec )
	{
        raw_eeg_per_page_duration = (int)total_recording_duration_sec;
	}   
    sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
	if (sel_raw_eeg_end_time > total_recording_duration_sec)
	{
		sel_raw_eeg_end_time = total_recording_duration_sec;
		sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
	}
	plot_raw_EEG_signal();	
}

void CEEG_marking_toolDlg::OnTimebase120() 
{
	// TODO: Add your command handler code here
	timebase = 120;		
    // raw EEG displays according to screen size and time base
    raw_eeg_per_page_duration = (int)(screen_x_size / (PixelsPerMM * timebase));   // (in Sec) time
    if ( raw_eeg_per_page_duration > total_recording_duration_sec )
	{
        raw_eeg_per_page_duration = (int)total_recording_duration_sec;
	}   
    sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
	if (sel_raw_eeg_end_time > total_recording_duration_sec)
	{
		sel_raw_eeg_end_time = total_recording_duration_sec;
		sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
	}
	plot_raw_EEG_signal();	
}

void CEEG_marking_toolDlg::OnTimebase240() 
{
	// TODO: Add your command handler code here
	timebase = 240;		
    // raw EEG displays according to screen size and time base
    raw_eeg_per_page_duration = (int)(screen_x_size / (PixelsPerMM * timebase));   // (in Sec) time
    if ( raw_eeg_per_page_duration > total_recording_duration_sec )
	{
        raw_eeg_per_page_duration = (int)total_recording_duration_sec;
	}   
    sel_raw_eeg_end_time = sel_raw_eeg_start_time + raw_eeg_per_page_duration;
	if (sel_raw_eeg_end_time > total_recording_duration_sec)
	{
		sel_raw_eeg_end_time = total_recording_duration_sec;
		sel_raw_eeg_start_time = sel_raw_eeg_end_time - raw_eeg_per_page_duration;
	}
	plot_raw_EEG_signal();	
}

void CEEG_marking_toolDlg::OnMarkingSeizure() 
{
	// TODO: Add your command handler code here
	mark_on = 0;
}

void CEEG_marking_toolDlg::OnMarkingBurst() 
{
	// TODO: Add your command handler code here
	mark_on = 1;
}

void CEEG_marking_toolDlg::OnMarkingArtifact() 
{
	// TODO: Add your command handler code here
	mark_on = 2;
}

void CEEG_marking_toolDlg::OnMarkingSleepspindle() 
{
	// TODO: Add your command handler code here
	mark_on = 3;
}

void CEEG_marking_toolDlg::OnMarkingSlowsharpwave() 
{
	// TODO: Add your command handler code here
	mark_on = 4;
}

void CEEG_marking_toolDlg::OnMarkingUnknownmark() 
{
	// TODO: Add your command handler code here
	mark_on = 5;
}

void CEEG_marking_toolDlg::OnMarkingUnmark() 
{
	// TODO: Add your command handler code here
	mark_on = 6;
}

void CEEG_marking_toolDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == VK_F2)	// F2 key is pressed - next
	{
		CEEG_marking_toolDlg::OnViewNext(0);
	}
	else if (nChar == VK_F3)	// F3 key is pressed - prev
	{
		CEEG_marking_toolDlg::OnViewPrev(0);
	}
	else if (nChar == VK_RIGHT)	// right arrow key is pressed - next
	{
		CEEG_marking_toolDlg::OnViewNext(1);
	}
	else if (nChar == VK_LEFT)	// left arrow key is pressed - prev
	{
		CEEG_marking_toolDlg::OnViewPrev(1);
	}
	else
	{
		CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

BOOL CEEG_marking_toolDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE)
			pMsg->wParam=NULL ;
	}
	if(pMsg->message==WM_KEYUP)
	{
		if(pMsg->wParam==VK_F2)
		{
//			AfxMessageBox("F2 Pressed......");
			CEEG_marking_toolDlg::OnViewNext(0);
		}
		else if(pMsg->wParam==VK_F3)
		{
//			AfxMessageBox("F3 Pressed......");
			CEEG_marking_toolDlg::OnViewPrev(0);
		}
		else if(pMsg->wParam==VK_RIGHT)
		{
			CEEG_marking_toolDlg::OnViewNext(1);
		}
		else if(pMsg->wParam==VK_LEFT)
		{
			CEEG_marking_toolDlg::OnViewPrev(1);
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CEEG_marking_toolDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	int i, count;
	struct channnel_data_list *temp;

	if (list_nodecount > 0)
	{
		for (count = 0; count < list_nodecount; count++)
		{
			temp = head;
			head = head->next;
			temp->prev = NULL;
			temp->next = NULL;
			if (head != NULL)
			{
				head->prev = NULL;
			}
			// for each channel, free its memory
			for (i = 0; i < no_of_channels; i++)
			{
				free(temp->channel_data[i]);
			}
			free(temp->channel_data);		// free channel data global pointer
		}
	}

	
	//initialize the marking database 
	if (marking_database != NULL)
	{
		for (i = 0; i < no_of_channels; i++)
		{
			for (count = 0; count < total_recording_duration_sec; count++)
			{
				free(marking_database[i][count]);
			}
			free(marking_database[i]);
		}
		free(marking_database);		// free marking database global pointer
	}

	CDialog::OnClose();
}

void CEEG_marking_toolDlg::OnFileExit() 
{
	// TODO: Add your command handler code here
	CEEG_marking_toolDlg::OnClose();
}
