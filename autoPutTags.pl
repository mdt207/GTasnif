#!/usr/pkg/bin/perl

#					Bismillahir Rahmanir Raheem
# All praises and gratitudes are due to Allah subhanahu wa Ta'ala
# O Allah: (please do) bless Muhammad (s.a.w) and the Household of Muhammad (s.a.w)

############################################################
# Muqobil Dasturlar To'plami (c) 2009-2019     		   #
# Purpose:                                                 #
# Date: started on 19.08.2019                		   #
# Released under the terms of GPL2       		   #
# Facilities: *BSD, GNUstep, mc, GNU/Linux, Perl5.14	   #
############################################################

#!/usr/pkg/bin/perl

use utf8;
#use v5.12;
use v5.14;
use autodie;
use strict;
use Encode;
#use warnings;
#use warnings qw( FATAL utf8);


sub put_tags_4_publisher
{
	my ($id, $str) = @_;
	
	if($id == 0)
	{
		#$str = '-' .$str;
		#say  $str;
		#if($str =~ s/(([\w])([\s+\.,:]+))([,\-\w]+)([\s+0-9]{5})/$1#$4#$5#/)
		#if($str =~ s/(([\w])([\s+\.,:]+))([,\-\w]+)([\s+0-9]{5})/#14$1##3$4##15$5#/)
		if( $str =~ s/ (?<pub_city> (?:\-)?([\s+A-ZА-Я]) ([\.:,\s+]+) ) (?<pub_name> [\-"a-zа-яA-ZА-Я,\s+]{3,}) (?<pub_year> [0-9]{4})/#14 $1##3 $4##15 $5#/ux )
		{
			#say $1.$4.$5;
			$str =~ y/[.,:]/ /;
			$str =~ s/\s+/ /;
			#say  $str;
			return $str;
		}
	}
	
	if($id == 1)
	{
		if( $str =~ s/(?<pages>[0-9]{1,}(\s+с)) /#13 $1#/uxs)
		{
			return $str;
		}
	}
	
	if($id == 2)
	{
		#if( $str =~ s/ ( \A([А-Я])([a-zA-Zа-яА-Я\s]{7,}) ([\.:\/]{1})  ) /#2 $1#/usx )
		if( $str =~ s/( ([a-zA-Zа-яА-Я\s]{9,})(?:[,:\/]{1})?  )/#2 $1#/uxs)
		{
			$str =~ y/[.,:]/ /;
			return $str;
		}
	}
	
}

my $inf  = $ARGV[0];
my $outf = $ARGV[1];

my $tagput_fh;
my $output_fh;

#'theAutoPutTags.txt'
if( $inf ne undef)
{
    open  TAGPUT, '<:encoding(UTF-8)', $inf
	#or die "Could not open theAutoPutTags.txt: $!";
}
else
{
    die "Could not open theAutoPutTags.txt: $!";
}

if( $outf ne undef)
{
 #'outPutFile.txt'
 open  OUTPUT, '>:encoding(UTF-8)', $outf
	#or die "Could not open outPutFile.txt: $!";
}
else
{
    die "Could not open outPutFile.txt: $!";
}
 
 my @pub_cities = (
							["Ташкент"],
							["Москва"],
							["Ленинград"],
						);
 
 my $concatenate_str;
 my $row_has_2_be_put;
 my $str;
 my $all_is_done = 0;
 my $author;
 my $pub_auth_sign;
 my $pages;
 my $title;
 my $coauthors;
 my $editor;
 my $translator;
 my $counter = 0;
 my $pub_city;
 my $pub_year;
 my $pub_name;
 my $surname_initial_word;
 
 while ( $row_has_2_be_put = <TAGPUT>)
 {
	$concatenate_str .= $row_has_2_be_put;
 }
 #$concatenate_str .= "\r";
 
 #print OUTPUT 'Hole str: '.$concatenate_str; 
 $row_has_2_be_put = $concatenate_str;
 #$row_has_2_be_put = "akjkjsdksjdksjd \t sfdfd \nfdffasd 1234  - 600 c\n";


if( $row_has_2_be_put =~ m/ (\/) (?<coauthors>[\w\s,\.]{3,} )  (\-?\s*[А-Я\.:]) /muxs )
{
  #$coauthors = $+{coauthors};
  #$coauthors = $1;
  
  #my @splitStr = split(/Под\.?\s*ред\.?/,$coauthors);
  
  #print OUTPUT "CoAuth : ".$+{coauthors}."\n";
  
  #unless($row_has_2_be_put =~ m/(ред)/muxs)
  if(length($+{coauthors}) > 5)
  {
	#print OUTPUT "CoAuth : ".$2."\n";
	  unless($2 =~ m/((Науч|Под)\.?\s*ред\.?) | ([\d]+?)/muxs)
	  {
		#print OUTPUT "CoAuth : ".$2."\n";
		if(length($2) > 5)
		{
			#print OUTPUT "CoAuth : ".$2."\n";
			$coauthors = " #5 ".$+{coauthors}." # ".$3;
			$row_has_2_be_put =~ s/ \/ ( [\w\s,\.]{3,} ) (\-?\s*[\w\.:])  / $coauthors /muxs;
		}
	  }
	  elsif($row_has_2_be_put =~ m/ (\/) (?<coauthors>[\w\s,\.]{3,} ) ((Науч|Под)\.?\s*ред\.?) /muxs)
	  {
		$coauthors = ' #5 '.$+{coauthors}.' # '.$3;
		$row_has_2_be_put =~ s/ (\/) ( [a-zA-Zа-яА-Я\s,\.]{3,} ) Под\.?\s*ред\.?  / $coauthors /muxs;
	  }
	  elsif( $row_has_2_be_put =~ m/ (\/) (?<coauthors>[\w\s,\.]{3,} )  (\-\s*[А-Я\.:])? /muxs )
	  {
		unless($2 =~ m/([\s\d]+?)/muxs)
		{
			if(length($2) > 5)
			{
				$coauthors = ' #5 '.$+{coauthors}.' # ';
				#print OUTPUT "CoAuth : ".$coauthors."\n";
				$row_has_2_be_put =~ s/ \/ ( [\w\s,\.]{3,} ) \-?\s*[А-Я\.:]  / $coauthors /muxs;
			}
		}
	  }
  }
}

if( $row_has_2_be_put =~ m/ ((Науч|Под)\.?\s*ред\.?) ([а-я\.\s]+)? (\s*[А-Я][\.,\s*])? (\s*[А-Я][\.,\s*])? \s* (?<editor>[\w,\.]{3,} )  (\s*[А-Я][\.\s*][^,\.])? (\s*[А-Я][\.\s*][^,\.])?  (\-?\s*[А-Я][\.:,])?  /muxs )
{
  $editor = $+{editor};
  
  $editor = $1.$3.$4.$5.$editor.$7.$8;
  #print OUTPUT "Editor : ".$editor."\n";
  
  $translator = $`;
  
	if($translator =~ m/(([Пп]ер)\.?\s*[cс]{1}\.?) ([\s\w\.]+?) (?<translator>[А-Я][а-я]{3,} (\s*[А-Я][\.,\s*])? (\s*[А-Я][\.,\s*])? ) /muxs)
	{
		$translator = $1.$3.$+{translator};
		#print OUTPUT "Translator : ".$translator."\n";
		$editor = $translator.$editor;
	}
  $editor = ' #11 '.$editor.' # ';
  #print OUTPUT "Editor : ".$9."\n";
  $row_has_2_be_put =~ s/ ((Науч|Под)\.?\s*ред\.?) ([а-я\.\s]+)? (\s*[А-Я][\.,\s*])? (\s*[А-Я][\.,\s*])? \s* (?<editor>[\w,\.]{3,} )  (\s*[А-Я][\.\s*][^,\.])? (\s*[А-Я][\.\s*][^,\.])?   / $editor /muxs;
  #$row_has_2_be_put =~ s/ ((Науч|Под)\.?\s*ред\.?) (\s*[А-Я][\.,\s*])? (\s*[А-Я][\.,\s*])? (?<editor>[\w,\s\-]{3,}) (\-?\s*[А-Я\.:,]) / $editor /muxs;
}

#if($row_has_2_be_put =~ m/(([Пп]ер)\.?\s*[cс]{1}\.?) ([\s\w\.]+?) (?<translator>[А-Я][а-я]{3,} (\s*[А-Я][\.,\s*])? (\s*[А-Я][\.,\s*])? ) /muxs)
#{
	#$translator = $1.$3.$+{translator};
	#if($editor ne undef)
	#{
		
		#$translator = $translator.$editor;
		#$translator =~ y/\#11/ /muxs;
		#$translator =~ s/\#11//muxs;
		#$translator =~ s/\s*//muxs;
		
		#print OUTPUT "Translator : ".$translator."\n";
		#print OUTPUT "Editor : ".$editor."\n";
		
		#$row_has_2_be_put =~ s/$editor/$translator /muxs;
		#$row_has_2_be_put =~ s/(([Пп]ер)\.?\s*[cс]{1}\.?) ([\s\w\.]+?) (?<translator>[А-Я][а-я]{3,} (\s*[А-Я][\.,\s*])? (\s*[А-Я][\.,\s*])? )/ #11 $1$3$+{translator} # /muxs;
	#}
#}
 
 if( $row_has_2_be_put =~ m/(\@\s*)(?<id_number>(?:[\d\.\-:\(\)]+))/muxs)
 {
    my $id_number = $+{id_number};
	#print OUTPUT "ID: ".$+{id_number}."\n";
	$row_has_2_be_put =~ s/(\@\s*)(?<id_number>(?:[\d\.\-:\(\)]+))/ #10 $id_number # /muxs;
 }
 
 if( $row_has_2_be_put =~ m/(ISBN)?\s*([I\s*В]{2,})?(?<isbn_number>(?:[\d\-A-Z]{9,}))/muxs)
 {
    my $isbn_number = $+{isbn_number};
    $isbn_number  = ' #9 '.$isbn_number.' # ';
    #print OUTPUT "ISBN: ".$isbn_number."\n";
    $row_has_2_be_put =~ s/(ISBN)?\s*([I\s*В]{2,})?(?<isbn_number>(?:[\d\-A-Z]{9,}))/ $isbn_number /muxs;
 }
 
 if( $row_has_2_be_put =~ m/(?:(ББК)|(\&)) \s* (?<pub_bbk> (?:[\d]{2}[\.]{1}[\dА-Я\(\)]{1,}) )/muxs)
 {
	my $pub_bbk = $+{pub_bbk};
	$pub_bbk = " #6 ".$pub_bbk.' # ';
	#print OUTPUT 'ББК: '.$+{pub_bbk}."\n";
	
	$row_has_2_be_put =~ s/(?:(ББК)|(\&)) \s* (?<pub_bbk> (?:[\d]{2}[\.]{1}[\dА-Я\(\)]{1,}) )/$pub_bbk/muxs;
 }
 
 if( $row_has_2_be_put =~ m/(?:(УДК)|(\^) ) \s* (?<pub_udk> (?:[\d\.\-:\(\)]{1,}) )/muxs)
 {
	my $pub_udk = $+{pub_udk};
	$pub_udk = " #7 ".$pub_udk.' # ';
	#print OUTPUT 'УДК: '.$pub_udk."\n";
	
	$row_has_2_be_put =~ s/(?:(УДК)|(\^))\s* (?<pub_udk> (?:[\d\.\-:\(\)]{1,}) )/$pub_udk/muxs;
 }
 
  
 #if( $row_has_2_be_put =~ m/( (\s*\-?\.?) (?<pub_city> ([А-Я]{1}\s* ))  ([\.:,"]{1,5}\s*) (?<pub_name> \s* [\w\-\.\s]{3,}) (,?\s*) (?<pub_year> [I\d]{4,5}\u) )/muxs )
 
 #if( $row_has_2_be_put =~ m/( (\s*\-?\.?) (?<pub_city> ([А-Я]{1} \s*)) ([\.:,"]{1,5}\s*) (?<pub_name> (\s*) ([А-Я][а-я]{3,}\s+[а-яА-Я\-\.\s]{3,})) ("?,{1,}\s?)? (?<pub_year> [\nI\d]{4,5}) )/muxs )
 #{
	#$pub_name = $+{pub_name};
	#$pub_city = $+{pub_city};
	#$pub_year = $+{pub_year};
	
	#say $str;
	#print OUTPUT $+{pub_city}.' '.$pub_name."\t".$+{pub_year}."\n";
	#print OUTPUT 'Publish '.$pub_name.' '.$pub_city.' '.$pub_year."\n";
	
	#$row_has_2_be_put =~ s/( (\s*\-?\.?) (?<pub_city> ([А-Я]{1}\s*)) ([\.:,"]{1,5}\s*) (?<pub_name> (\s*)([\w\-\.\s]{3,})) ("?,{1,}\s?)? (?<pub_year> [\nI\d]{4,5}) )/ #14 $+{pub_city} # #3 $+{pub_name} # #15 $+{pub_year} #/muxs;
 #}
 
# if($pub_name eq undef) and ($pub_year eq undef)
 {
	if( $row_has_2_be_put =~ m/( (\s*\-?\.?,?) (?<pub_city> [А-Я]{1}[\s*\.:,]{1,4} ) ([\.:,"«]{1,4}\s*) (?<pub_name> \s* [\w\-\.\s]{3,} ) (»?"?,?\s*) (?<pub_year> [\nI\d]{4,5}) )/muxs )
	{
		$pub_name = $+{pub_name};
		$pub_city = $+{pub_city};
		$pub_year = $+{pub_year};
	
	
		#print OUTPUT 'Publish '.$pub_city.' '.$pub_name.' '.$pub_year." \n";
		
		$row_has_2_be_put =~ s/( (\s*\-?\.?,?) (?<pub_city> [А-Я]{1}[\s*\.:,]{1,4} ) ([\.:,"«]{1,4}\s*) (?<pub_name> \s* [\w\-\.\s]{3,} ) (»?"?,?\s*) (?<pub_year> [\nI\d]{4,5}) )/ #14 $pub_city # #3 $pub_name # #15 $pub_year #/muxs;
		
		#$row_has_2_be_put =~ s/( (\s*\-?\.?) (?<pub_city> [А-Я]{1}[\.:,"]{1,4} ) ([\.:,"]{1,4}) (?<pub_name> \s* [\w\-\.\s]{3,} ) (,?\s*) (?<pub_year> [\nI\d]{4,5}) )/ #14 $pub_city # #3 $pub_name # #15 $pub_year #/muxs;
	 }
}
 
 if(($pub_city eq undef) or ($pub_year eq undef))
 {
	if( $row_has_2_be_put =~ m/( (\s*\-?\.?) (?<pub_city> ([А-Я]{1} \s*)) ([\.:,"]{1,5}\s*)  (?<pub_year> [\nI\d]{4,5}) )/muxs )
	{
		#print OUTPUT "City:".$+{pub_city}.' '.$+{pub_year}."\n"; 
		$row_has_2_be_put =~ s/( (\s*\-?\.?) (?<pub_city> ([А-Я]{1}\s* )) ([\.:,"]{1,5}\s*) (?<pub_year> [\nI\d]{4,5}) )/ #14 $+{pub_city} # #15 $+{pub_year} #/muxs;
	}
 }


 if( $row_has_2_be_put =~ m/Изд-во"?( (?<pub_name_ex>(?:[\w]+)) )/imuxs)
 { 
	 my $city;
	 my $i = 0;
	 
	 #say "Publishing: ".$+{pub_name_ex};
	 #$pub_year = $1;
	 $pub_name = $+{pub_name_ex};
	 
	 if($row_has_2_be_put =~ m/([\nI\d]{4,5})[\s*\-]{1,3}/muxs)
	 {
		#print OUTPUT "Year: ".$1."\n"; 
		$row_has_2_be_put =~ s/([\nI\d]{4,5})[\s*\-]{1,3}/ #15 $1 # /muxs;
	 }
	 #$row_has_2_be_put =~ s/(\d{4}\u)[\s\-]/ #15 $pub_year # /muxs;
	 
	 $row_has_2_be_put =~ s/Изд-во"?( (?<pub_name_ex>(?:[\w]+)) )/ #3 $pub_name # /imuxs;
	 
	 for $i (0..$#pub_cities)
	 {
		 #say $pub_cities[$i][0]."\n";
		 if($row_has_2_be_put =~ m/$pub_cities[$i][0]/imuxs)
		 {
			 $city = $pub_cities[$i][0];
			 #say $pub_cities[$i][0];
			 $city = substr($city,0,1);
			 $row_has_2_be_put =~ s/$pub_cities[$i][0]/ #14  $city # /imuxs;
		 }
	 }
	 
	 #print substr($city,0,1)."\n";
	 
 } 
 
 if($pages eq undef)
	 { 
		if( $row_has_2_be_put =~ m/(?<pages>([0-9]{1,4})(\s*)([сc][\s\.,]{1,}) ) /muxs)
		{
			$pages = $+{pages};
			#print OUTPUT 'Pages: '.$pages."\n";
		
			#say 'pages: '.$+{pages};
		
			#$str = $1;
			#$str = put_tags_4_publisher( 1, $pages );
			
			$row_has_2_be_put =~ s/(?<pages>([0-9]{1,4})(\s*)([сc][\s\.,]{1,}) ) / #13 $+{pages} #/muxs;
			
			#print $output_fh  $row_has_2_be_put ;
			#print $output_fh  $`."  ".put_tags_4_publisher( 1, $str )." ".$';
			#print $output_fh  put_tags_4_publisher( 1, $row_has_2_be_put );
			
			#$all_is_done = 1;
			#$counter++;
			
			#print OUTPUT $row_has_2_be_put;
		}
	 }


if($author eq undef)
	{
		$all_is_done = 0;
		$str = undef;
		
		#if( $row_has_2_be_put =~ m/( ( [A-ZА-Яa-zа-я,]{3,}(?:\s) ) [А-Я]{1} (?:\.\s?)? (?:[А-Я]{1} (?:\.\s?))? (?:[а-я,]{2,})?  )/muxs)

		#if( $row_has_2_be_put =~ m/ ( ([A-ZА-Я]{1}[a-zа-я]{3,}) (,?\s?)  (?:[А-Я]{1}\.?)?  (?:\s?[А-Я]{1}\.?)? (\w+)? ) /muxs)
		if( $row_has_2_be_put =~ m/( ([A-ZА-Я][a-zа-я]{3,}) \s* (,?[A-ZА-Я]\.?[^а-я]) \s* ([А-Я],?\.?[^а-я])? )/muxs)
		
		#if( $row_has_2_be_put =~ m/(?<author> ([A-ZА-Яa-zа-я\s,]{3,} ) ([А-Я]{1} (?:\.\s)? ([а-я,]+)?) )/uxs)
		{
			$author = $1;
			$author =~ s/^\s+//;
			$surname_initial_word = substr($author, 0, 1);
			
			#chomp($author);
			#print OUTPUT 'Author: '.$author."\n";
			#print OUTPUT 'CoAuthor: '.$'."\n";
			
			if($coauthors eq undef)
			{
				$coauthors = $';
			
				if( $coauthors =~ m/( ([A-ZА-Я][a-zа-я]{3,}) \s* (,?[A-ZА-Я]\.?[^а-я]) \s* ([А-Я],?\.?[^а-я])? )/muxs)
				{
					$str = $1;
					#print OUTPUT 'CoAuthor: '.$str."\n";
					$coauthors = ' #5 '.$str.' # ';
				}
			}
			$author = "#1 ".$author." # ";
			if($str ne undef)
			{
				$author = $author.$coauthors;
			}
		    
			#print OUTPUT 'Author: '.$author."\n";
			#say 'Author: '.$author;
			
			$row_has_2_be_put =~ s/([A-ZА-Я]{1}[a-zа-я]{3,} (,?\s?[A-ZА-Я]\.) (\s?[A-ZА-Я\.]{1,2})?) /$author/muxs;
			
			if($str ne undef)
			{
				if($row_has_2_be_put =~ m/([^#\d]\s?,?)($str)/mus)
				{
					#print OUTPUT 'CoAuthor:'.$str."\n";
					$row_has_2_be_put =~ s/ ([^#\d]\s?,?)($str)//u;
				}
			}
			
			#$row_has_2_be_put =~ s/ ( ([A-ZА-Я]{1}[a-zа-я]{3,}) (,?\s?)  (?:[А-Я]{1}\.?)?  (?:\s?[А-Я]{1}\.?)? (\w+)? ) /$author/muxs;
			
			#$row_has_2_be_put =~ s/ ( ([A-ZА-Я]{1}[a-zа-я]{3,}) (,?\s?)  (?:[А-Я]{1}\.?)? (?:\s*)? (?:[А-Я]{1}\.?)? (\w+)? ) /$author/muxs;

			#$row_has_2_be_put =~ s/( ( [A-ZА-Яa-zа-я,]{3,}(?:\s) ) [А-Я]{1} (?:\.\s?)? (?:[А-Я]{1} (?:\.\s?))? (?:[а-я,]{2,})?  )/$author/muxs;
			#$row_has_2_be_put =~ s/( ( [A-ZА-Яa-zа-я\s,]{3,} ) ([А-Я]{1} (?:\.\s)? ([а-я,]+)?) ([А-Я.]{1}) ([А-Я]{1} (?:.)?)  )/$author/uxs;
			#$row_has_2_be_put =~ s/( ( [A-ZА-Яa-zа-я\s,]{3,} ) ([А-Я]{1} (?:\.\s)? ([а-я,]+)?) )/$author/uxs;
			
			#$all_is_done = 1;
		}
		if($author eq undef) 
		{
		  if( $row_has_2_be_put =~ m/( ([А-Я][а-я]{3,})(,?\s?)([А-Я][а-я]{3,}) \s* ([а-яА-Я]{3,})? )/muxs )
		  {
		    $author = $1;
			#print OUTPUT 'CoAuthor: '.$'."\n";
			
			if($coauthors eq undef)
			{
				$coauthors = $';
				if( $coauthors =~ m/( ([А-Я][а-я]{3,})(,?\s?)([А-Я][а-я]{3,}) \s* ([а-яА-Я]{3,})? )/muxs )
				{
					$str = $1;
					#print OUTPUT 'CoAuthor: '.$str."\n";
					$coauthors = ' #5 '.$str.' # ';
				}
			}
			
			$surname_initial_word = substr($author, 0, 1);
			
			#print OUTPUT 'Author3: '.$author."\n";
			
			$author = "#1 ".$author." # ";
			if($str ne undef)
			{
				$author = $author.$coauthors;
			}
			$row_has_2_be_put =~ s/ ([а-яА-Я]{3,}(,?\s?)([а-яА-Я]{3,}) \s* ([а-яА-Я]{3,})? ) /$author/muxs;
			
			if($str ne undef)
			{
				if($row_has_2_be_put =~ m/([^#\d]\s)($str)/mus)
				{
					#print OUTPUT 'CoAuthor:'.$str."\n";
					$row_has_2_be_put =~ s/ ([^#\d]\s)($str)//u;
				}
			}
			
		  }
		}
		if($author eq undef) 
		{
			if( $row_has_2_be_put =~ m/( (,?[А-Я]\.) \s* (,?[A-ZА-Я]\.)? \s* ([A-ZА-Я][a-zа-я]{3,}) )/muxs)
			{
				$author = $1;
				#print OUTPUT 'Author2: '.$author."\n";
				$author = "#1 ".$author." # ";
				$row_has_2_be_put =~ s/( (,?[А-Я]\.) \s* (,?[A-ZА-Я]\.)? \s* ([A-ZА-Я][a-zа-я]{3,}) ) / $author /muxs;
			}
		}
	}
	

#if( $row_has_2_be_put =~ m/(?<pub_auth_sign>( ([А-Я]\u{1}\s*\.?\d{2,})([^:\.\d+\w+\-]\n?) ) )/muxs)
if($surname_initial_word ne undef)
{
	#print OUTPUT "Auth sign: ".$surname_initial_word."\n";

	if( $row_has_2_be_put =~ m/(?<pub_auth_sign>( ($surname_initial_word\s*\.?\d{2,})([^:\.\d+\w+\-]\n?) ) )/muxs)
	 {
		$pub_auth_sign = $+{pub_auth_sign};
		$pub_auth_sign = " #16 ".$pub_auth_sign." # ";
		#print OUTPUT "Auth sign: ".$+{pub_auth_sign}."\n";
		
		$row_has_2_be_put =~ s/(?<pub_auth_sign>( ($surname_initial_word\s*\.?\d{2,})([^:\.\d+\w+\-]\n?) ) )/ $pub_auth_sign /muxs
	 }
}


if($editor eq undef)
{
	#if( $row_has_2_be_put =~ m/Пер[\.\s]{1,2} ([а-я\s]{3,7})(\.)? (\s*)[А-Я](\.?[А-Я]\.?) /muxs)
	#if( $row_has_2_be_put =~ m/(Пер[\.\s]{1,2}) ([а-я\s]{3,7})(\.)? (\s*)([а-яА-Я\s\.]+)(\.?) /muxs)
	if($row_has_2_be_put =~ m/(([Пп]ер)\.?\s*[cс]{1}\.?) ([\s\w\.]+?) (?<translator>[А-Я][а-я]{3,} (\s*[А-Я][\.,\s*])? (\s*[А-Я][\.,\s*])? ) /muxs)
	{
		$translator = $1.$3.$+{translator};
		#print OUTPUT "Trans: ".$translator."\n";
		$row_has_2_be_put =~ s/(([Пп]ер)\.?\s*[cс]{1}\.?) ([\s\w\.]+?) (?<translator>[А-Я][а-я]{3,} (\s*[А-Я][\.,\s*])? (\s*[А-Я][\.,\s*])? )/ #11 $translator # /muxs;
		#$row_has_2_be_put =~ s/(Пер[\.\s]{1,2}) ([а-я\s]{3,7})(\.)? (\s*)([а-яА-Я\s\.]+)(\.?)/ #11 $5 # /muxs;
	}
}
	 
if( $title eq  undef )
	 {
			#print OUTPUT 'Title: '.$row_has_2_be_put."\n";
			
			#if( $row_has_2_be_put =~ /( \A([А-Я])([a-zA-Zа-яА-Я\s]{7,}) ([\.:\/]{1})  )/uxs)
			#if( $row_has_2_be_put =~ m/( ((\R) (?:[а-яА-Я0-9]+)? (\s+) [a-zA-Zа-яА-Я\s]{3,} (?:[\.,:\/]{1})?) )/uxs)
			#if( $row_has_2_be_put =~ m/ \R ([\w\s]+\u)? [\w\s]*\u [\.,:\/]{1} /muxs)
			
			#if( $row_has_2_be_put =~ m/\R  ([а-яА-Я]\u{1,3}[0-9]{1,3})? (?<title>    ( [а-яА-Я,\s]{4,}\u)   ) (?:[\.:\/]{1})?/muxs)
			#if( $row_has_2_be_put =~ m/ ( (?:[А-Я\d]{1,3})? ([А-Я][а-я\s]{4,}\u)  (?:[\.:\/]{1})? ) /muxs)
			#if( $row_has_2_be_put =~ m/ (?<title> ([А-Я][а-я]{2,}\s+[а-я\*,\s]{3,}[а-яА-Я\-]* )  [^:а-яА-Я][а-яА-Я]+  ) /muxs)
			
			#if( $row_has_2_be_put =~ m/(?<title> [А-Я][а-я]{2,}[\s+][:а-я\*\s\.]{3,}  [а-яА-Я\s\.\-]+ ) (Пер)? /gmuxs)
			
			if( $row_has_2_be_put =~ m/([А-Я][а-я\-\s]{2,}) ([а-яА-Я\d\s:]+)? (?<title> \s+[:а-я"«,\*\s]{3,} [\dа-яА-Я»"\s\-,]+  ) ([\/\s\.])?  /muxs)
			{
                #print OUTPUT 'Title: '.$3."\n";
				$title = $1.$2.$+{title}.$4;
				
				if($title =~ m/:/muxs)
				{
					my @splitStr = split(/:/, $title);
				
					if( length($splitStr[1]) < 7 )
					{
						$title =~ s/$splitStr[1]//uxs;
						#print OUTPUT 'Title: '.$splitStr[1]."\n";
						$title =~ s/://muxs;
						#print OUTPUT 'Title: '.$title."\n";
					}
				}
				#say 'Title:'.$+{title};
				
				#print OUTPUT 'Title: '.$1.' '.$+{title}.' '.$4."\n";
				
				#my $match = $&;
				#my $tmp = $';
	   
				#if( $tmp =~ /( ([a-zA-Zа-яА-Я\s]{7,}) ([:\/]{1})  )/uxs)
				#{
				#	say 'Title2: '.$match.$1;
				#	$str = $match.$1;
				#}
				#else
				#{
				#	$str = $1;
				#}
				
				#$title = put_tags_4_publisher( 2, $1 );
				
				#$row_has_2_be_put =~ s/([a-zA-Zа-яА-Я\s]{9,})(?:[,:\/]{1})?/ $title /uxs;
				
				#$row_has_2_be_put =~ s/( \A([А-Я])([a-zA-Zа-яА-Я\s]{7,}) ([\.:\/]{1})  )/$title/uxs;
				
				#$title = $1.$+{title}.$4;
				$title =~ s/\s*//uxs;
				$title = " \n#2 ".$title." # ";
				#print OUTPUT 'Title: '.$title."\n";
				
				
				$row_has_2_be_put =~ s/([А-Я][а-я\-\s]{2,}) ([а-яА-Я\d\s:]+)? (?<title> \s+[:а-я"«,\*\s]{3,} [\dа-яА-Я»"\s\-,]+ ) ([\/\s\.])? /$title/muxs;
				
				#$row_has_2_be_put =~ s/ (?<title> ([А-Я][а-я]{2,}\s+[а-я\*,\s]{3,}[а-яА-Я\-]* ) [\.\-,\s:а-яА-Я]{2,} ) /$title/muxs;
				
				#$row_has_2_be_put =~ s/(?<title> ([А-Я][а-я]{2,}\s+[а-я\*,\s]{3,}[а-яА-Я\-]* )  [^:а-яА-Я][а-яА-Я]+  ) /$title/muxs;
				
				#$row_has_2_be_put =~ s/ (?<title> ([а-яА-Я\s]{4,}\s+[а-яА-Я\-\s]{3,}) ) [^\d+] (?:[,\.:\/]{1})? /$title/muxs;
				
				
				#$row_has_2_be_put =~ s/ \R  ([\s#\d]*)? ([а-яА-Я]\u{1,3}[0-9]{1,3}[#\s]+)? (?<title> ([а-яА-Я\-\n\s+]{4,}\u) ) (?:[,\.:\/]{1})? (?:[^А-Я#\.]{1,})?/ $title /muxs;
				
				#$row_has_2_be_put =~ s/ \R (?:.\s+)? (?:[а-яА-Я]\u{1,3}[0-9]{1,3})?  ([а-яА-Я\-\n\s]{4,}\u)  (?:[,\.:\/]{1})? (?:[^А-Я#\.]{1,})? / $title /muxs;
				
				#print OUTPUT 'Title: '.$title."\n";
				#$row_has_2_be_put =~ s/ \R (?:[а-яА-Я]{1,3}[0-9]{1,3})? ([а-яА-Я\s]{4,}\u)  (?:[\.,:\/]{1})? /$title/muxs;
				
				#print $output_fh  $row_has_2_be_put ;
				#print $output_fh  $`."  ".put_tags_4_publisher( 2, $str )." ".$';
				#print $output_fh  put_tags_4_publisher( 2, $row_has_2_be_put );
		
				#$all_is_done = 1;
				#$counter++;
			}
	 }	 
 
 
 
 
 #while ( $row_has_2_be_put = <TAGPUT>)
 while ( $counter > 5)
 { 
	 #chomp($row_has_2_be_put);
	 
	 if( $row_has_2_be_put =~ m/(\/[A-ZА-Яa-zа-я\s\.,]{3,}\-)/uxs )
	 {
		 $coauthors = "#5 ";
		 $str = $1;
		 #say 'Coauthors: '.$1;
		 
		 #while (  (index($str, "Под") > 1 || index($str, "под") > 1) ) # || (index($str, "ред") > 1 || index($str, "Ред") > 1) ) 
		 while( $str =~ m/([A-ZА-Яa-zа-я]{3,}) (\s)? ([А-Я](\.)) ([А-Я](\.))/uxs )
		 {
			#if(  index($str, "Под") < 1 || index($str, "под") < 1)  { last; }
			if( $str =~ s/([A-ZА-Яa-zа-я]{3,}) (\s)? ([А-Я](\.)) ([А-Я](\.))//uxs )
			{
				#say index($str, "Под");
				#say $str;
				$coauthors .= $1.' '.$3.$5.' ';
				#say 'SubCoAuthors: '.$1.$3.$5;
			}
		}
		 $coauthors .= "#";
		 #say 'SubCoAuthors: '.$coauthors;
		 $row_has_2_be_put =~ s/(\/[A-ZА-Яa-zа-я\s\.,]{3,}\-)/$coauthors/uxs;
		 #$row_has_2_be_put .= $';
		 $all_is_done = 1;
		 $counter++;
	 }
	 
	if($author eq undef)
	{
		my @fields;
		my $i = 0;
		
		if( $row_has_2_be_put =~ m/( ( [A-ZА-Яa-zа-я,]{3,}(?:\s) ) ([А-Я]{1} (?:\.\s)? ([а-я,]+)?) (?:[А-Я]{1} (?:\.)?)? (?:[А-Я]{1} (?:\.)?)? )/uxs)
		#if( $row_has_2_be_put =~ m/(?<author> ([A-ZА-Яa-zа-я\s,]{3,} ) ([А-Я]{1} (?:\.\s)? ([а-я,]+)?) )/uxs)
		{
			#print OUTPUT 'Author: '.$1."\n";
			
			$author = "#1 ";
		    my $s = $row_has_2_be_put;
			#$all_is_done = 1;
			if( ( index($s, 'И') > 0 || index($s, 'и') > 0 ) && $all_is_done == 0)
			{	
				#print OUTPUT 'Author: '.$s." ".index($s, 'и')."\n";
				
				@fields = split /[Ии]{1}(?:\s)/, $s;
				
				for $i (0..$#fields)
				{
					if( $fields[$i] =~ m/([A-ZА-Я][a-zа-я.]{2,})/uxs )
					{
						#print OUTPUT $i." ".$fields[$i]."\n";
						if($i == 0)
						{
							$author .= $fields[$i]."#";
							$all_is_done = 1;
							$counter++;
						}
						if($i == 1)
						{
							
						}
					}
				}
			}
			
			elsif( index($s,",") > 0 && $all_is_done == 0 )
			{
				#print OUTPUT "TEST\n";
				
				@fields = split /[,]{1}(?:\s+)?/, $s;
				for $i (0..$#fields)
				{
					if( $fields[$i] =~ m/([A-ZА-Я][a-zа-я.]{2,})/uxs )
					{
						#print OUTPUT $i." ".$fields[$i]."\n";
						if($i == 0)
						{
							$author .= $fields[$i]."#";
							$all_is_done = 1;
							$counter++;
						}
						if($i == 1)
						{
							
						}
					}
				}
			}
			
			if( $all_is_done == 0 )
			{
				#$author .= $+{author}.'#';
				$author .= $1."#";
				$all_is_done = 1;
				$counter++;
			}
			print OUTPUT 'Author: '.$author."\n";
			
			$row_has_2_be_put =~ s/( ( [A-ZА-Яa-zа-я\s,]{3,} ) ([А-Я]{1} (?:\.\s)? ([а-я,]+)?) (?:[А-Я.]{1})? (?:[А-Я]{1} (?:.)?)?  )/$author/uxs;
			#$row_has_2_be_put =~ s/( ( [A-ZА-Яa-zа-я\s,]{3,} ) ([А-Я]{1} (?:\.\s)? ([а-я,]+)?) ([А-Я.]{1}) ([А-Я]{1} (?:.)?)  )/$author/uxs;
			#$row_has_2_be_put =~ s/( ( [A-ZА-Яa-zа-я\s,]{3,} ) ([А-Я]{1} (?:\.\s)? ([а-я,]+)?) )/$author/uxs;
			
			#$all_is_done = 1;
		}
	}
	 
	 if( ($title eq  undef) && ($row_has_2_be_put =~ m/^[\sА-Я\.]/) )
	 {
			#print OUTPUT 'Title: '.$row_has_2_be_put."\n";
			
			#if( $row_has_2_be_put =~ /( \A([А-Я])([a-zA-Zа-яА-Я\s]{7,}) ([\.:\/]{1})  )/uxs)
			if( $row_has_2_be_put =~ m/( (?:[\s]+)? ([a-zA-Zа-яА-Я\s]{7,}) (?:[\.,:\/]{1})? )/uxs)
			{
				print OUTPUT 'Title: '.$1."\n";
				#my $match = $&;
				#my $tmp = $';
	   
				#if( $tmp =~ /( ([a-zA-Zа-яА-Я\s]{7,}) ([:\/]{1})  )/uxs)
				#{
				#	say 'Title2: '.$match.$1;
				#	$str = $match.$1;
				#}
				#else
				#{
				#	$str = $1;
				#}
				
				$title = put_tags_4_publisher( 2, $1 );
				#print OUTPUT 'Title: '.$title."\n";
				
				$row_has_2_be_put =~ s/([a-zA-Zа-яА-Я\s]{9,})(?:[,:\/]{1})?/ $title /uxs;
				
				#$row_has_2_be_put =~ s/( \A([А-Я])([a-zA-Zа-яА-Я\s]{7,}) ([\.:\/]{1})  )/$title/uxs;
				
				#print $output_fh  $row_has_2_be_put ;
				#print $output_fh  $`."  ".put_tags_4_publisher( 2, $str )." ".$';
				#print $output_fh  put_tags_4_publisher( 2, $row_has_2_be_put );
		
				$all_is_done = 1;
				$counter++;
			}
	 }
	  
	 #if($pages eq undef)
	 { 
		#if( $row_has_2_be_put =~ m/(?<pages>[0-9\s]{2,} )(?:[с](\.)?)/uxs)
		{
			#$pages = $+{pages};
			#print OUTPUT 'Pages: '.$+{pages}."\n";
		
			#say 'pages: '.$+{pages};
		
			#$str = $1;
			#$str = put_tags_4_publisher( 1, $str );
			#$row_has_2_be_put =~ s/( ([0-9\s]{2,}) ([с](\.?)) )/$str/uxs;
			
			#print $output_fh  $row_has_2_be_put ;
			#print $output_fh  $`."  ".put_tags_4_publisher( 1, $str )." ".$';
			#print $output_fh  put_tags_4_publisher( 1, $row_has_2_be_put );
			
			#$all_is_done = 1;
			#$counter++;
		}
	 }
	 
	if( $row_has_2_be_put =~ m/ (?<pub_city> (?:\-)?([\s+A-ZА-Я]) ([\.:,\s+]+) ) (?<pub_name> [\-"a-zа-яA-ZА-Я,\s+]{3,}) (?<pub_year> [0-9]{4})/uxs
	#14 $+{pub_city} #3 $+{pub_name} #15 $+{pub_year}
	 )
	#([\s+| , | \. | \- | \s+] [0-9]{4})/ux  )
	{
	    #chomp ($2);
		#print "gotcha: $1$3$4\n";
		#say "#14 ".$+{pub_city}."#3 ".$+{pub_name}."#15 ".$+{pub_year};
		
		#say "Publish: ".$+{pub_city}.$+{pub_name}.$+{pub_year};
		
		# 3 $str = $+{pub_city}.$+{pub_name}.$+{pub_year};
		# 2 $str = put_tags_4_publisher( 0, $str );
		
		#say $str;
		
		# 1 $row_has_2_be_put =~ s/ (?<pub_city> ([\-\s+A-ZА-Я]) ([\.:,\s+]+) ) (?<pub_name> [\-"a-zа-яA-ZА-Я,\s+]{3,}) (?<pub_year> [0-9]{4})/$str/usx;
		
		#print $output_fh  $row_has_2_be_put ;
		#print $output_fh  $`."  ".put_tags_4_publisher( 0, $str )." ".$';
		#print $output_fh  put_tags_4_publisher( 0, $row_has_2_be_put );
		
		#say $`."  ".$&." ".$';
		
		#say $`."  ".put_tags_4_publisher( 0, $str )." ".$'; 
		
		
		#$str2 = $+{pub_name};
		#$str =~ y/[.,:]/ /;
		#if($str2 =~ s/[\s+.,:]+/ #/)
		#{
			#say $str2;
		#}
		
		#$str .= '#';
		
		#$str = $4;
		#say $_;
		#say $row_has_2_be_put;
		$all_is_done = 1;
		$counter++;
	}
	
	 if( $all_is_done > 0 )
	 {
		 $all_is_done = 0;
		 #print OUTPUT  $row_has_2_be_put ;
	 }
	 else
	 {
		 #print OUTPUT  $row_has_2_be_put ;
	 }
 }
 
	if( $all_is_done > 0 )
	{
		 $all_is_done = 0;
	}
 print OUTPUT  $row_has_2_be_put."\n";
 print OUTPUT 'Counter: '.$counter;
 
#$_="- Т.:	\"Мир\",  1980.	-";
#$_ = "\t- М.: \"Наука\", 1971. -";

#if(/([\-\s+]+[A-Z][\s+\.:]+)([a-zA-Z]+)([\s+|,|\.|\-|\s+0-9]*)/)

#if( /([\-\s+]+[A-ZА-Я][\s+\.:]+)([\Wa-zа-яA-ZА-Я]+)([\s+|,|\.|\-|\s+0-9]*)/u)
#{
#	chomp ($1);
	#utf8::upgrade($1);
	#print "gotcha: $3\n";
	#say "gotcha:".$1.$2.$3;
#}
#else
#{
		#print $_."\n";
#}

close TAGPUT;
close OUTPUT;
