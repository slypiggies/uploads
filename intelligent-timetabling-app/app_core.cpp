#include "app_core.h"
using namespace std;
auto rande = default_random_engine(chrono::system_clock::now().time_since_epoch().count());
const int N = 1005, LEN = 105, PDAY = 900, PWEEK = PDAY * 7 + 5;
const string WEEK[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
char s[N][LEN];
int a[N], pid[N], rpid[N];
int pord[N], pslot[N];
vector<int> rord;
vector<pair<int, int>> slot[N], tslot[N];
int seg[N];
int na[N], nseg[N];
vector<int> req[N], par[N];
int n, ans_val;
vector<pair<int, int>> ans, vec;
deque<pair<int, int>> mnq;
int freq[PWEEK], prefreq[PWEEK];
int setu[PWEEK];
vector<pair<int, pair<bool, bool>>> warning;
bool dead[N];
int vis[N];
vector<int> med;
string file_name, export_dir, warning_dir;
FILE *file_name_txt;

void scand(int i) {
    fscanf(file_name_txt, "%d%*c", pid + i);
    rpid[pid[i]] = i;
    for (int j = 0;; j++) {
        char ch;
        fscanf(file_name_txt, "%c", &ch);
        if (ch == ',') break;
        s[i][j] = ch;
        s[i][j + 1] = '\0';
    }
    int x, y;
    fscanf(file_name_txt, "%d%*c%d%*c%d", a + i, &x, &y);
    seg[i] = x * 60 + y;
    char t[10];
    fgets(t, 10, file_name_txt);
    int m, r;
    fscanf(file_name_txt, "%d%d", &m, &r);
    for (int j = 0; j < m; j++) {
        int day;
        fscanf(file_name_txt, "%s", t);
        for (int k = 0; k < 7; k++) if (strcmp(WEEK[k].c_str(), t) == 0) {
            day = k;
            break;
        }
        fscanf(file_name_txt, "%d%*c%d%*c", &x, &y);
        x = (x - 9) * 60 + y + PDAY * day;
        slot[i].push_back(make_pair(x, 0));
        fscanf(file_name_txt, "%d%*c%d%*c", &x, &y);
        x = (x - 9) * 60 + y + PDAY * day;
        slot[i].back().second = x - 1;
    }
    for (int j = 0; j < r; j++) {
        fscanf(file_name_txt, "%d%*c", &x);
        req[i].push_back(x);
    }
}

void dfs1(int i) {
    vis[i]++;
    if (vis[i] > 1) return;
    for (int j : req[i]) dfs1(j);
}

void dfs2(int ori, int i) {
    for (int j : req[i]) {
        par[ori].push_back(j);
        dfs2(ori, j);
    }
}

void fix_data() {
    for (int i = 0; i < n; i++) {
        for (int &j : req[i]) j = rpid[j];
    }
    for (int i = 0; i < n; i++) {
        memset(setu, 0, sizeof setu);
        for (pair<int, int> j : slot[i]) {
            setu[j.first]++;
            setu[j.second + 1]--;
        }
        for (int j = 1; j < PWEEK; j++) setu[j] += setu[j - 1];
        for (int j = 0; j < PWEEK; j++) if (setu[j] > 0) {
            int k = j;
            for (; k < PWEEK && setu[k] > 0; k++);
            k--;
            if (k - j + 1 >= seg[i]) tslot[i].push_back(make_pair(j, k));
            j = k;
        }
        slot[i] = tslot[i];
    }
}

void check_data() {
    for (int i = 0; i < n; i++) {
        memset(vis, 0, sizeof vis);
        dfs1(i);
        pair<bool, bool> bad(slot[i].empty(), vis[i] > 1);
        if (bad.first || bad.second) {
            warning.push_back(make_pair(pid[i], bad));
            dead[i] = true;
            if (bad.first) {
                slot[i].push_back(make_pair(0, 0));
                seg[i] = 1;
            }
        }
    }
}

void init() {
    for (int i = 0; i < n; i++) if (!dead[i]) for (pair<int, int> j : slot[i]) {
        for (int k = j.first; k <= j.second; k++) freq[k]++;
    }
    prefreq[0] = freq[0];
    for (int i = 1; i < PWEEK; i++) prefreq[i] = freq[i] + prefreq[i - 1];
    for (int i = 0; i < n; i++) if (!dead[i]) dfs2(i, i);
    for (int i = 0; i < n; i++) {
        na[i] = a[i];
        nseg[i] = seg[i];
        for (int j : par[i]) {
            na[i] += a[j];
            nseg[i] += seg[j];
            dead[i] |= dead[j];
        }
    }
}

void ord2() {
    for (int i = 0; i < n; i++) pord[i] = i;
    sort(pord, pord + n, [](int x, int y) {
        return 1.0 * na[x] / nseg[x] > 1.0 * na[y] / nseg[y];
    });
}

void ord2a(int dist = 2) {
    ord2();
    for (int i = 0; i + dist - 1 < n; i++) {
        shuffle(pord + i, pord + i + dist, rande);
        shuffle(pord + n - i - dist, pord + n - i, rande);
    }
}

void slot1() {
    for (int i = 0; i < n; i++) {
        int rec = INT_MAX;
        for (int j = 0; j < slot[i].size(); j++) {
            mnq.clear();
            int mn = INT_MAX;
            for (int k = slot[i][j].first; k < slot[i][j].first + seg[i] - 1; k++) {
                mnq.push_back(make_pair(freq[k], k));
            }
            for (int k = slot[i][j].first + seg[i] - 1; k <= slot[i][j].second; k++) {
                while (!mnq.empty() && mnq.back().first >= freq[k]) mnq.pop_back();
                mnq.push_back(make_pair(freq[k], k));
                while (!mnq.empty() && mnq.front().second < k - seg[i] + 1) mnq.pop_front();
                mn = min(mn, mnq.front().first);
            }
            if (mn < rec) {
                rec = mn;
                pslot[i] = j;
            }
        }
    }
}

void slot2() {
    for (int i = 0; i < n; i++) {
        int rec = INT_MAX;
        for (int j = 0; j < slot[i].size(); j++) {
            pair<int, int> t = slot[i][j];
            int sum = prefreq[t.second];
            if (t.first > 0) sum -= prefreq[t.first - 1];
            if (sum < rec) {
                rec = sum;
                pslot[i] = j;
            }
        }
    }
}

void slot3() {
    for (int i = 0; i < n; i++) {
        double rec = INT_MAX;
        for (int j = 0; j < slot[i].size(); j++) {
            pair<int, int> t = slot[i][j];
            double sum = 0;
            for (int k = t.first; k + seg[i] - 1 <= t.second; k++) {
                sum += prefreq[k + seg[i] - 1];
                if (k > 0) sum -= prefreq[k - 1];
            }
            double mean = sum / (t.second - seg[i] + 2 - t.first);
            if (mean < rec) {
                rec = mean;
                pslot[i] = j;
            }
        }
    }
}

void slot4(bool flag = false) {
    for (int i = 0; i < n; i++) {
        int rec = INT_MAX;
        pair<int, int> rec2(INT_MAX, 0);
        for (int j = 0; j < slot[i].size(); j++) {
            med.clear();
            pair<int, int> t = slot[i][j];
            for (int k = t.first; k + seg[i] - 1 <= t.second; k++) {
                med.push_back(prefreq[k + seg[i] - 1]);
                if (k > 0) med.back() -= prefreq[k - 1];
            }
            sort(med.begin(), med.end());
            int median = med[int(med.size()) / 2];
            if (!flag) {
                if (median < rec) {
                    rec = median;
                    pslot[i] = j;
                }
            } else {
                if (median < rec) {
                    rec2 = make_pair(rec, pslot[i]);
                    rec = median;
                    pslot[i] = j;
                } else if (median < rec2.first) {
                    rec2 = make_pair(median, j);
                }
            }
        }
        if (rande() % 2) pslot[i] = rec2.second;
    }
}

void greedy() {
    rord.clear();
    for (int i = 0; i < n; i++) {
        rord.push_back(pord[i]);
        for (int j : par[pord[i]]) rord.push_back(j);
        sort(rord.begin(), rord.end(), [](int x, int y) {
            pair<int, int> t1 = slot[x][pslot[x]], t2 = slot[y][pslot[y]];
            return t1.first < t2.first || t1.first == t2.first && t1.second < t2.second || t1 == t2 && x < y;
        });
        rord.resize(unique(rord.begin(), rord.end()) - rord.begin());
        int t = 0;
        vec.clear();
        memset(vis, 0, sizeof vis);
        for (int j = 0, k = 0; j < rord.size(); j++) {
            int now = rord[j];
            if (dead[now]) continue;
            bool gg = false;
            for (int l : par[now]) gg |= !vis[l];
            if (gg) continue;
            auto res = upper_bound(slot[now].begin(), slot[now].end(), make_pair(k, 0));
            if (res != slot[now].begin()) {
                res--;
                if (k + seg[now] - 1 <= res->second) {
                    t += a[now];
                    vec.push_back(make_pair(now, k));
                    vis[now] = true;
                    k += seg[now];
                    goto skip;
                }
                res++;
                if (res != slot[now].end()) {
                    t += a[now];
                    vec.push_back(make_pair(now, res->first));
                    vis[now] = true;
                    k = res->first + seg[now];
                }
                skip:;
            } else {
                t += a[now];
                vec.push_back(make_pair(now, res->first));
                vis[now] = true;
                k = res->first + seg[now];
            }
        }
        if (t > ans_val) {
            ans_val = t;
            ans = vec;
        }
    }
}

void print_ans(bool basic_ans) {
    FILE *export_dir_txt = fopen(export_dir.c_str(), "w");
    if (basic_ans) {
        fprintf(export_dir_txt, "%d\n", ans.size());
        for (pair<int, int> i : ans) {
            fprintf(export_dir_txt, "%d %s %02d:%02d\n", pid[i.first], WEEK[i.second / PDAY].c_str(), i.second % PDAY / 60 + 9, i.second % PDAY % 60);
        }
    } else {
        fprintf(export_dir_txt, "%d activity(s) can be done:\n", ans.size());
        for (pair<int, int> i : ans) {
            fprintf(export_dir_txt, "%s on %s at %02d:%02d.\n", s[i.first], WEEK[i.second / PDAY].c_str(), i.second % PDAY / 60 + 9, i.second % PDAY % 60);
        }
        fprintf(export_dir_txt, "Score: %d\n", ans_val);
    }
    fclose(export_dir_txt);
}

void solve(bool basic_ans) {
    auto lucky = clock();
    lucky += 1.2 * CLOCKS_PER_SEC;
    file_name_txt = fopen(file_name.c_str(), "r");
    fscanf(file_name_txt, "%d", &n);
    for (int i = 0; i < n; i++) scand(i);
    fclose(file_name_txt);
    fix_data();
    check_data();
    init();
    ord2(); slot1(); greedy();
    slot2(); greedy();
    slot3(); greedy();
    slot4(); greedy();
    while (clock() <= lucky) {
        ord2a(); slot4(); greedy();
        ord2a(); greedy();
        ord2(); slot4(true); greedy();
        slot4(true); greedy();
        ord2a(); slot4(true); greedy();
        ord2a(); slot4(true); greedy();
        ord2a(); slot4(true); greedy();
        ord2a(); slot4(true); greedy();
    }
    print_ans(basic_ans);
}

void warn() {
    FILE *warning_txt = fopen(warning_dir.c_str(), "w");
    int j = 0;
    for (auto i : warning) {
        j++;
        fprintf(warning_txt, "%d. Activity %d cannot be considered due to", j, i.first);
        if (i.second.first && i.second.second) fprintf(warning_txt, " A) No period available, and B) Prerequisite(s) can never be finished.\n");
        if (i.second.first) fprintf(warning_txt, " A) No period available.\n");
        if (i.second.second) fprintf(warning_txt, " A) Prerequisite(s) can never be finished.\n");
    }
    fclose(warning_txt);
}

void reset_all() {
    memset(s, 0, sizeof s);
    memset(a, 0, sizeof a); memset(pid, 0, sizeof pid); memset(rpid, 0, sizeof rpid);
    memset(pord, 0, sizeof pord); memset(pslot, 0, sizeof pslot);
    rord.clear();
    for (int i = 0; i < N; i++) {slot[i].clear(); tslot[i].clear();}
    memset(seg, 0, sizeof seg);
    memset(na, 0, sizeof na); memset(nseg, 0, sizeof nseg);
    for (int i = 0; i < N; i++) {req[i].clear(); par[i].clear();}
    n = 0; ans_val = 0;
    ans.clear(); vec.clear();
    mnq.clear();
    memset(freq, 0, sizeof freq); memset(prefreq, 0, sizeof prefreq);
    memset(setu, 0, sizeof setu);
    warning.clear();
    memset(dead, 0, sizeof dead); memset(vis, 0, sizeof vis);
    med.clear();
}

int go(QString fileName, bool readableFormat, QString exportDir) {
    reset_all();
    file_name = fileName.toStdString();
    export_dir = exportDir.toStdString() + "/result.txt";
    warning_dir = exportDir.toStdString() + "/warning.txt";
    solve(!readableFormat);
    warn();
    return int(warning.size());
}
