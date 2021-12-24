#!/usr/bin/perl

#					Bismillahir Rahmanir Raheem
# All praises and gratitudes are due to Allah subhanahu wa Ta`ala
# O Allah: (please do) bless Muhammad (s.a.w) and the Household of Muhammad (s.a.w)

############################################################
# Muqobil Dasturlar To'plami (c) 2009-2017     		   #
# Purpose: Ugly way of converting sorted cards into ISIS   #
# Date: started on 04.04.2009                		   #
# Released under the terms of GPL3       		   #
# Facilities: *BSD, Xfce4, mc, GNU/Linux, Perl5.8	   #
############################################################
# it's up to You to clean up the source

#use Switch; #better not to use

#system("clear");#UNICES
#system("cls");#WNT

@pat = (
         ["#100:   a20031117d","    |||y0rusy02     "],
	 ["#101: 0 arus"],
	 ["#102:   "],
         ["#105:   ay   z   000#y"],
	 ["#200: 1 a"],			#Title
	 ["#210:   a","c","d"],		#City, year
	 ["#215:   a"],
	 ["#320:   a"],
	 ["#686:   a", "2rugasnti"],
	 ["#700:  1a", "b"],		#author
	 ["#701:  1a"],			#coauthors
	 ["#801:  0bMuqobil DasturlaraUZc20090407"],
       );

if($ARGV[0] =~ /-h/) 
 {
   print "Muqobil Dasturlar To'plami (c) 2009-2017\nCould be used under the terms of BSDl or GPL3\n";
   print "\nUsage: card2isis.pl <path1> <path2>\n
	path1: sorted with GTasnif file location
	path2: where & under what name to save formatted file\n"; 
   exit;#say goodbay
 }

print "Muqobil Dasturlar To'plami (c) 2009-2017\n";
print "--------------------------------------------\n";
print "Converting started...\n";

$inf  = $ARGV[0]; #"view.txt"; should be Win EOL (CRLF)
#$inf = "cards.txt";
$outf = $ARGV[1];#"irbis.txt";
$unimarkf = "unimark.txt";
#$unimarkf = "view.txt";

print $inf." ".$outf."\n";

open(FP, "<$inf") || die "can't open $inf: $!";
open(OP, ">$outf") || die "can't open $outf: $!";

my( @card_data,  @tmp ,  @unimark_pat, %uhash);

$i=0;
$str;

#TKL K96 207 added
$main_sentinel = "TKLK96207";# 
$sentinel = '\*\*\*\*';
$CR_LF = "\n"; #"\x0D\x0A"

#open(UNIFP, "<$unimarkf") || die "can't open $inf: $!";


sub read_unimark_pat
{
  open(UNIFP, "<$unimarkf") || die "can't open $inf: $!";
   
   my $i = 0;
   $#unimark_pat = -1;
   #@unimark_pat = <UNIFP>;
   while ( my $str = <UNIFP>)
   {
      #print $str;
      #if($str =~ /$sentinel/)
      #{ $unimark_pat[$i] = [$str];}
      #@tmp = split /\s+/, $str, 2;#match the 1 space
      if($str =~ /#801:/) 
       { 
          $str = $pat[11][0].$CR_LF;#sign up 
       }
      $str =~ /\s{1}/;#touch the space once
      $tmp[0] = $`." "; $tmp[1] = $'; 
      #print $tmp[0]."\n";
      #$unimark_pat[$i] =  [@tmp];
      #print $unimark_pat[$i][0];
      push @unimark_pat, [@tmp]; 
      $i++;
   }

   #remove the space behind the stars
   --$i;
   $unimark_pat[$i][0] =~s/\s$/$3/g;

   #for $i (0..$#unimark_pat) 
   #{
     #print  @{$unimark_pat[$i]};
      #print $unimark_pat[$i][0];
   #}

 close (UNIFP);   

 # for $aref ( @unimark_pat )
 #  {
  #    print "PAt:@$aref";     
  # }

}

sub read_card
{
  #print "\n";
  LOOP: while( ($str = <FP>) )
  {
    
    #$str = $str.$CR_LF;

    #$str =~s/\W$/$1/g; #clean up

    $str =~s/\s$/$1/g; #clean up
    
    #print "$str\n";
    if( $str =~ /$sentinel/ )
    {
      #print "$str\n";
       read_unimark_pat;  
       $i = 0;
       #last LOOP;
       #print $unimark_pat[1][0];
       
 
       for $i (0..$#unimark_pat) 
       {
         #$tmp[$i] = [@unimark_pat];
         #print "$i:"; 
         #print  "-----1@{$unimark_pat[$i]}"; 
         #print "Res:".$res[$i][1];
          #while (($key, $value) = each %uhash)
          foreach $key(sort(keys %uhash))
           { 
             #print "$key = $uhash{$key}\n";
             $unimark_pat[$i][1] =~ s/$key/$uhash{$key}/g;
           }
          #print @{$unimark_pat[$i]};
          #(print OP $unimark_pat[$i][0].$unimark_pat[$i][1]);
       }
       #clean up, remove the tags
       for $i (0..$#unimark_pat) 
       { 
           $unimark_pat[$i][1] =~ s/#\d+:/$3/g;
	   #print "Clean up";
           #print $unimark_pat[$i][1];
           (print OP $unimark_pat[$i][0].$unimark_pat[$i][1]);  
       }

        (print OP $CR_LF);
        %uhash = {}; #level it to ruins, to raise it again        
    }
    else 
    { 
      $str =~ /\s+/;#match the space
      #print $`.$'."\n";   
      $uhash{$`.":"} = $'; #key setted to string before space, value with string after it 
       
    }

      #@tmp = split( /\s+/, $str );
      #print $tmp[0]."\n";
      #$uhash{$tmp[0]} = $tmp[1].$tmp[2].$tmp[3];       

  }
  
     
  #print "TEST:".$uhash{"#14:"};
}

read_unimark_pat;
read_card;

close (FP);# done with cards file
close (OP);# done with cards file
#close (UNIFP);   

print "\nDone!";
