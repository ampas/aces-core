% General Class Information
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{academydoc}[2015/01/01 Academy Document class]

% Load Packages
\LoadClassWithOptions{report}
\RequirePackage[hmargin={1.25in,1.25in},vmargin={1.0in,1.0in}]{geometry}
\RequirePackage{graphicx} % For Images
\graphicspath{{./images/}} % Path to images
\RequirePackage{changepage} % To allow for indented margins
\RequirePackage{ragged2e} % For Justified Text
\RequirePackage{helvet} % For Helvetica San Serifs
\RequirePackage{times} % For Times Serifs
\RequirePackage{titlesec} 
\RequirePackage{tabularx}
\RequirePackage{fancyhdr}
\RequirePackage{etoolbox}
\RequirePackage{tocloft} % Use to format the TOC
\RequirePackage{tocvsec2} % Use to get rid of the terms from the TOC
\RequirePackage[titletoc,title]{appendix}
\RequirePackage{chngcntr}
\RequirePackage{hyperref}
\RequirePackage{mathtools}
\RequirePackage{float}
\RequirePackage{ifthen}
\RequirePackage[format=plain,font=bf,labelsep=endash]{caption}
\RequirePackage[hang,flushmargin]{footmisc} 
\RequirePackage{enumitem}
\RequirePackage{amsfonts}
\RequirePackage{tabto}
\RequirePackage{listings}
\RequirePackage{pdfpages}

%\usepackage{showframe}

% Document Formatting
%\defaultfontfeatures{Ligatures=TeX}
\pagenumbering{arabic}
\renewcommand{\familydefault}{\rmdefault} % Set default roman font to times

\setlength{\parskip}{6pt}
\setlength{\parindent}{0pt}
\renewcommand{\baselinestretch}{1.5}
\linespread{1.0}

\makeatletter
\patchcmd{\chapter}{\if@openright\cleardoublepage\else\clearpage\fi}{\par\vspace{0.3in}}{}{} % Removes page break from chapter format
\makeatother

\def\chapterautorefname{Section}
\def\sectionautorefname{Section}
\def\subsectionautorefname{Section}

\newcommand\prelimsectionformat
{
    \setcounter{secnumdepth}{-1}
    \titleformat{\chapter}{\sffamily\fontsize{10}{12}\bfseries}{\thechapter}{0.2in}{}
    \titlespacing*{\chapter}{0pt}{-20pt}{0pt}
}

\newcommand\termsanddefsectionformat
{
    \titleformat{\section}{\fontfamily{ptm}}{\thesection}{0.2in}{}
    \titlespacing*{\section}{0pt}{0pt}{0pt}

    \settocdepth{chapter}
}
    
\newcommand\regularsectionformat
{
    \titleformat{\chapter}{\sffamily\fontsize{16}{19.2}\bfseries}{\thechapter}{0.2in}{}
    \titlespacing*{\chapter}{0pt}{-20pt}{0pt} % No indentation after the title
    \titleformat{\section}{\sffamily\fontsize{14}{16.8}\bfseries}{\thesection}{0.2in}{}
    \titlespacing*{\section}{0pt}{6pt}{0pt} % No indentation after the title
    \titleformat{\subsection}{\sffamily\fontsize{10}{12}\bfseries}{\thesubsection}{0.2in}{}
    \titlespacing*{\subsection}{0pt}{6pt}{0pt} % No indentation after the title
    \titleformat{\subsubsection}{\sffamily\fontsize{10}{12}\bfseries}{\thesubsubsection}{0.2in}{}
    \titlespacing*{\subsubsection}{0pt}{6pt}{0pt} % No indentation after the title

    \settocdepth{subsection}
}

\newcommand{\appendixsectionformat}[1]
{    
    \ifthenelse{\equal{#1}{n}}{
        \titleformat{\chapter}[display]
        {\sffamily\fontsize{14}{20}\bfseries}
        {\centering Appendix \Alph{chapter}\\*\vspace{6pt}\fontsize{10pt}{12pt}\mdseries\selectfont(normative)}
        {0in}
        {\vspace{2pt}\centering}        
    }{
        \titleformat{\chapter}[display]
        {\sffamily\fontsize{14}{20}\bfseries}
        {\centering Appendix \Alph{chapter}\\*\vspace{6pt}\fontsize{10pt}{12pt}\mdseries\selectfont(informative)}
        {0in}
        {\vspace{2pt}\centering}        
    }
    \titlespacing*{\chapter}{0pt}{-20pt}{0pt} % No indentation after the title
}

% Usage is \appendix{nameOfSection}{i/n(informative/normative)}
\newcommand{\appendixchapter}[2]
{
	\cleardoublepage

    \settocdepth{section}
    \appendixsectionformat{#2}
    \chapter{#1}
    \vspace{8pt}
}

% Table of Contents Formatting
\renewcommand*\contentsname{Table of Contents}

\renewcommand{\cfttoctitlefont}{\sffamily\fontsize{16}{19.2}\bfseries} % Table of Contents Title Font
\renewcommand{\cftchapfont}{\fontfamily{ptm}} % Chapter Font
\renewcommand{\cftchappagefont}{\fontfamily{ptm}} % Page Number Font

\cftsetindents{chapter}{0em}{1.5em}  % Indenting
\cftsetindents{section}{1.5em}{2.3em}
\cftsetindents{subsection}{3em}{3.2em}

\setlength{\cftbeforetoctitleskip}{0pt} % Vertical spacing
\setlength{\cftaftertoctitleskip}{0pt}
\setlength{\cftbeforechapskip}{6pt}
\setlength{\cftbeforesecskip}{6pt}
\setlength{\cftbeforesubsecskip}{6pt}

\renewcommand{\cftchapleader}{\cftdotfill{\cftdotsep}} % Add dot leader
\cftsetpnumwidth{12pt} % Adjust margin btween leader and page number

% Headers and footers
\fancypagestyle{plain}{%
  \fancyhf{} % clear all six fields
  \fancyhead[L]{\Docnumber}
  \fancyhead[C]{\Altdocname}
  \fancyhead[R]{Page \thepage}
  \fancyfoot[L]{Version \Versionnumber}
  \fancyfoot[R]{\Docdate }
  \renewcommand{\headrulewidth}{0pt}
  \renewcommand{\footrulewidth}{0pt}
}

% Table formatting
\renewcommand{\arraystretch}{1.5}    
\counterwithout{table}{chapter}

% Figure formatting
\counterwithout{figure}{chapter}

% Equation formatting
\newfloat{floatequ}{H}{loe}
\floatname{floatequ}{Equation}
\captionsetup[floatequ]{font=bf,labelfont=bf,labelsep=endash}

\makeatletter
\renewcommand\tagform@[1]{\relax}
\makeatother

    
\floatname{floatequ}{Equation}
\captionsetup[floatequ]{font=bf,labelfont=bf,labelsep=endash}

\makeatletter
\renewcommand\tagform@[1]{\relax}
\makeatother

% Code snippet formatting (e.g. CTL)
\lstset{
  tabsize=2,
  columns=fixed,
  basicstyle={\ttfamily\fontsize{8}{9.6}\selectfont}
}

% Define custom metadata commands
\newcommand{\doctype}[1]{\newcommand\Doctype{#1}}
\newcommand{\docname}[1]{\newcommand\Docname{#1}}
\newcommand{\altdocname}[1]{\newcommand\Altdocname{#1}}
\newcommand{\docnumber}[1]{\newcommand\Docnumber{#1}}
\newcommand{\committeename}[1]{\newcommand\Committeename{#1}}
\newcommand{\versionnumber}[1]{\newcommand\Versionnumber{#1}}
\newcommand{\docdate}[1]{\newcommand\Docdate{#1}}
\newcommand{\summary}[1]{\newcommand\Summary{#1}}
\newcommand{\noticestext}[1]{\newcommand\Noticestext{#1}}
\newcommand{\revisiontable}[1]{\newcommand\Revisiontable{#1}}
\newcommand{\ampasdocstable}[1]{\newcommand\Ampasdocstable{#1}}
\newcommand{\acknowledgementstable}[1]{\newcommand\Acknowledgementstable{#1}}
\newcommand{\introductiontext}[1]{\newcommand\Introductiontext{#1}}
\newcommand{\listoftermsanddefs}[1]{\newcommand\Listoftermsanddefs{#1}}

% Set format for bulleted lists
\let\olditemize\itemize
\renewcommand{\itemize}{
    \vspace{-6pt}
    \olditemize[leftmargin=0.25in]
    \setlength{\itemsep}{6pt}
    \setlength{\parskip}{0pt}
    \setlength{\parsep}{0pt}
    \setlength{\topsep}{0pt}
}

\newenvironment{listize}[1][]{
    \begin{list}{#1}{
        \setlength{\itemsep}{6pt}
        \setlength{\parskip}{0pt}
        \setlength{\parsep}{0pt}
        \setlength{\topsep}{6pt}
    }
}{
    \end{list}
}

% Set format for enumerated lists
\let\oldenumerate\enumerate
\renewcommand{\enumerate}{
    \vspace{-6pt}
    \oldenumerate[leftmargin=0.25in]
    \setlength{\itemsep}{6pt}
    \setlength{\parskip}{0pt}
    \setlength{\parsep}{0pt}
    \setlength{\topsep}{0pt}
}

% Redefine \maketitle for creating the title page
\renewcommand{\maketitle}
{
    % Insert Academy Logo
    \thispagestyle{empty}
    \begin{figure}[t]
	    \vspace*{0.5in}
        \includegraphics[width=0.75in]{AMPAS_Logo_Legal.png} % Need a better image
        \centering
	    \vspace{0.5in}
    \end{figure}

	\begin{center}
	    \fontsize{18}{21.6} 
		\selectfont

        \ifthenelse{\equal{\Doctype}{spec}}{
            \sffamily{\textbf{Specification}} \\
        }{
            \ifthenelse{\equal{\Doctype}{tb}}{
                \sffamily{\textbf{Technical Bulletin}} \\
            }{
                \ifthenelse{\equal{\Doctype}{proc}}{
                    \sffamily{\textbf{Procedure}} \\
                }{
                    \sffamily{\textbf{}} \\
                }
            }
        }
		\vspace{12pt}
		\sffamily{\textbf{\Docnumber}} \\ 
		\vspace{12pt}
        \sffamily{\textbf{\Docname}} \\

		\vspace{22pt}
		\fontsize{14}{16.8} 
		\selectfont

		\rmfamily{The Academy of Motion Picture Arts and Sciences} \\
		\vspace{6pt}
		Science and Technology Council \\
		\vspace{6pt}
		\Committeename \\

		\vspace{22pt}
		\fontsize{11}{13.2} 
		\selectfont
		Version \Versionnumber \hspace{0.25 in} \Docdate \\
		\vspace{18pt}
	\end{center}
	\begin{adjustwidth}{0.5in}{0.5in}
        \begin{justify}
            \fontsize{11}{13.2} 
            \selectfont
            \setlength{\parindent}{0pt}
            Summary: \Summary
        \end{justify}
	\end{adjustwidth}
	\cleardoublepage
}

\newcommand{\unnumberedformat}
{
    \regularsectionformat
    \setcounter{secnumdepth}{-1}
}

\newcommand{\numberedformat}
{
    \regularsectionformat
    \setcounter{secnumdepth}{9}
}

\newcommand{\term}[1]
{
    \termsanddefsectionformat
    
    \section{#1}
    
    \regularsectionformat
}

% Make a note macro that autoresets with subsection or above 
\newcounter{note}

\makeatletter
\@addtoreset{note}{chapter}
\@addtoreset{note}{section}
\@addtoreset{note}{subsection}
\@addtoreset{note}{subsubsection}
\makeatother


\newcommand{\note}[1]
{
    \ifthenelse{ \arabic{note} > 0}{
	    NOTE \arabic{note}: #1
	}{
	    NOTE: #1
	    \stepcounter{note}
	}
	\stepcounter{note}
}

% Formatting for XML elements
\def\xmlitem[#1][#2]#3{\item[{\texttt{#1}}] \tabto{0.5\textwidth}(#2) \\ #3}
\def\xmlitemopt[#1][#2]{\item[{\texttt{#1}}]  \tabto{0.5\textwidth}(#2)}
\def\xmlitemd[#1]#2{\item[{\texttt{#1}}] \hfill \\ #2}

\newenvironment{xmlfields}{
	\renewcommand*\descriptionlabel[1]{\hspace\labelsep\texttt{##1}}
	\begin{description}[itemindent=-0.2in,leftmargin=0.5in,itemsep=6pt,topsep=0pt]
}{	
	\end{description}
	\vspace{6pt}
}	
