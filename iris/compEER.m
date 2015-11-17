function [] = compEER()
%
%  Verification Mode in Biometric System
% Equal Error Rate (EER) -   FAR = FRR
%   lower EER -> better system
%

% Type I error:  False Accept Rate (FAR)  ---  FAR

% Type II error: False Rejection Rate  --- FRR
%

%Read the data from the result - compare.txt
name = 'E:\share\ZWL\1112\compare_cr_lamp.txt';
nuser = 100 ; 
nid = nuser * 2;  % 100 identities * (right eye + left eye)
nsamp = 20; % 20 samples in each eye

%ndis = nid * nsamp; % the dim
%rest = zeros(nid, nsamp*2, ndis);
%sret = zeros(nid * nsamp * nsamp, 1);  % for the same identity
%dret = zeros(nid * (nid -1) * nsamp * nsamp, 1); % for the different identities
sret = {};
dret = {};

fid = fopen(name, 'rt');

m =0;  % the line num
sn = 0; % count the same identity
dn = 0;

%eye = 'R'; % flag
expression = '(\d+[LR])(\d*)';

while feof(fid) == 0,
    m = m+1;
    line = fgetl(fid);
    sline = regexp(line, ' ', 'split');
    
    % the 1rt identity -
    % id1 = char(regexp(sline{1}, '\d*', 'match'));
    res1 = regexp(sline{1}, expression , 'tokens');
    user1 = char(res1{1}{1});    % form the format S2001L02 - >2001L
    id1 =str2num( res1{1}{2}); % form the format S2001L02 - >2
      
      % the 2nd identity -
     res2 = regexp(sline{2}, expression , 'tokens');
     user2 = char(res2{1}{1});
     id2 =str2num( res2{1}{2});  % form the format S2001L02 - > 2001
       
     % the distance value
     dist = str2num(sline{3});
   
%      if m == 20 * 2 * 100,
%          fprintf('\n');
%      end
     
     if user1 == user2,  % the same identity
          sn =  sn + 1;
          sret{sn} = dist;
     else
         dn = dn + 1;
         dret{dn} = dist;
     end
     
      
      % the compared identities
      %cid = char(regexp(sline{2}, '\d*', 'match'));
      %cuser = str2num(cid(2,:));  % left eyes are first, then  right eyes
      
%       cuser = cuser + 1; %  the compared identities = mod( cuser ,(nsamp*2))
%       dim = mod(cuser, nid * nsamp );
%       if  dim == 0, %each 4000 (nuser * nsamp) has a new identity
%           dim = nid * nsamp; 
%       end
      
%       if comp == nuser * nsamp,
%            fprintf('\n');
%       end
% 
%    %check whether the compared object is right eye
%    reye_ = char(regexp(sline{2}, eye, 'match'));
%    if reye_ == eye,
%         %id_ = char(regexp(sline{2}, '\d*', 'match'));
%         %samp_ =  str2num(id_(2,:));
%         samp = samp + nsamp;  % add
%    end
%       
%      % check whether it is right eye
%      reye = char(regexp(sline{1}, eye, 'match'));
%      if  reye == eye, % right eye
%          %
%          user = user + nuser; % consider the user identity 1 as 101
%     end
      
%      % treat right eye of one person with another identity
%      mcomp = mod (m,  nsamp * ndis); % samps, each identity has 40 samples (left + right ones)
%      if mcomp ==  0, %each 80000 = 4000 * 20 (sample)
%           % flag the right eye
%           flag = 1;
%          %samp = samp + 20; % for right eye
%      end

      
       % matrix res
%       rest(user, samp, dim) = dis; 
       
end

% Equal Error Rate (EER) :  FAR = FRR
[FARs, FRRs, thred] = comp( sret, dret );

% plot
 figure(1); hold on;
 %     subplot(211)
title('ERR Curve');
axis([0, 1, 0, 100]);
% plot(thred, FARs, 'b--+', 'MarkerSize',8);
% plot(thred, FRRs, 'r--o', 'MarkerSize',8);
line(thred, FARs*100,'LineWidth',2,'Color','b');
%text(thred, FARs*100, 'FAR');
line(thred, FRRs*100,'LineWidth',2,'Color','r');
%text(thred, FRRs*100, 'FRR');

%plot(Te,er/2, 'g*'); % fla

%hold on
xlabel('Threshold Value, Ts', 'FontSize', 14);
ylabel('Error Rate (%)', 'FontSize', 14);

h = legend( 'FAR', 'FRR',2, 'Location', 'southeast');
set(h, 'FontSize', 10, 'position', [0.7, 0.12, 0.2, 0.2]);


% Te  can be determined as the trade-off point
% 
function [FARs, FRRs, thred, Te, er] = comp( sd, dd )
     intra_dists=  cell2mat(sd);
     inter_dists = cell2mat(dd);
     a = min(intra_dists);
     b = max(inter_dists);
      
     a = min(sd);
      b = max(dd);
      dx = (b - a) / 100;
      
      thred = 0:dx:1;
      
      Te = 0; 
      er=0; % Error Rate
      FARs = [];
      FRRs = [];
      
      for x=0:dx:1,
              er1=length(intra_dists(intra_dists<=x))/length(intra_dists);
              er2=length(inter_dists(inter_dists>x))/length(inter_dists);
              
              FARs=[FARs er1];
              FRRs=[FRRs er2];
              
            % if er1 == er2,  %Equal Error Rate (EER) :  FAR = FRR
            if abs(er1 -er2) <= dx,
                 %er=er1;
                 er=er1+er2;
                 
                 % best threshold 
                 Te=x;
            end
      end

